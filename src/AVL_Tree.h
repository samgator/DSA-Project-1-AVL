#pragma once
using namespace std;
#include <iostream>
#include <iomanip>
#include <vector>

class Node {
public:
    int id;
    string name;
    int height;
    Node* left;
    Node* right;
    Node(int id, string name) : id(id), name(name), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree {
private:
    Node* root;

    // Helper functions
    int height(Node* node) {
        if (!node) {
            return 0;
        }
        return node->height;
    }

    int getBalanceFactor(Node* node) {
        if (!node) {
            return 0;
        }
        return height(node->left) - height(node->right);
    }

    Node* getMinNode (Node* node) {
        Node* currNode = node;

        while (currNode->left) {
            currNode = currNode->left;
        }
        return currNode;
    }

    Node* rotateLeft(Node* node) {
        Node* rightChild = node->right;
        Node* grandchild = rightChild->left;

        rightChild->left = node;
        node->right = grandchild;

        node->height = 1 + max(height(node->left), height(node->right));
        rightChild->height = 1 + max(height(rightChild->left), height(rightChild->right));

        return rightChild;

    }

    Node* rotateRight(Node* node) {
        Node* leftChild = node->left;
        Node* grandchild = leftChild->right;

        leftChild->right = node;
        node->left = grandchild;

        node->height = 1 + max(height(node->left), height(node->right));
        leftChild->height = 1 + max(height(leftChild->left), height(leftChild->right));

        return leftChild;

    }

    // insert command
    Node* insertNode(Node* node, string name, int id, bool& successfulInsert) {
        if (!node){
            successfulInsert = true;
            return new Node(id, name);
        // sort by ID
        } else if (id < node->id) {
            node->left = insertNode(node->left, name, id, successfulInsert);
        } else if (id > node->id){
            node->right = insertNode(node->right, name, id, successfulInsert);
        } else {
            successfulInsert = false;
            return node;
        }

        // balance tree
        node->height = 1 + max(height(node->left), height(node->right));

        int balanceFactor = getBalanceFactor(node);

        if (balanceFactor < -1) {
            if (getBalanceFactor(node->right) < 0) {
                return rotateLeft(node);
            } else if (getBalanceFactor(node->right) > 0) {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }

        if (balanceFactor > 1) {
            if (getBalanceFactor(node->left) > 0) {
                return rotateRight(node);
            } else if (getBalanceFactor(node->left) < 0) {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }

        return node;
    }

    // remove command
    Node* deleteNode(Node* node, int id, bool& successfulDeletion) {
        if (!node) {
            return node;
        }

        if (id < node->id) {
            node->left = deleteNode(node->left, id, successfulDeletion);
        } else if (id > node->id) {
            node->right = deleteNode(node->right, id, successfulDeletion);
        } else {
            successfulDeletion = true;

            if ((!node->left) || (!node->right)) {
                Node* tempNode = node->left ? node->left : node->right;

                if (!tempNode) {
                    tempNode = node;
                    node = nullptr;
                } else {
                    *node = *tempNode;
                     delete tempNode;

                }

            } else {
                Node* tempNode = getMinNode(node->right);
                node->id = tempNode->id;
                node->name = tempNode->name;
                node->right = deleteNode(node->right, tempNode->id, successfulDeletion);
            }
        }

        if (!node) {
            return node;
        }

        // balance tree
        node->height = 1 + max(height(node->left), height(node->right));

        int balanceFactor = getBalanceFactor(node);

        if (balanceFactor < -1) {
            if (getBalanceFactor(node->right) <= 0) {
                return rotateLeft(node);
            } else {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }

        if (balanceFactor > 1) {
            if (getBalanceFactor(node->left) >= 0) {
                return rotateRight(node);
            } else {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }

        return node;
    }

    // removeInorderN command
    Node* deleteNInOrder(Node* node, int n, bool& successfulDeletion, int& index) {
        if (!node) {
            return node;
        }

        node->left = deleteNInOrder(node->left, n, successfulDeletion, index);
        index++;
        if (index == n) {
            node = deleteNode(node, node->id, successfulDeletion);
            return node;
        }
        node->right = deleteNInOrder(node->right, n, successfulDeletion, index);

        return node;

    }
    

    // traversals
    void inOrderTraversal(Node* node, vector<string>& list) {
        if (node) {
            inOrderTraversal(node->left, list);
            list.push_back(node->name);
            inOrderTraversal(node->right, list);
        }
    }

    // for use in Test Case 5
    void inOrderTraversalIDs(Node* node, vector<string>& list) {
        if (node) {
            inOrderTraversalIDs(node->left, list);
            list.push_back(to_string(node->id));
            inOrderTraversalIDs(node->right, list);
        }
    }

    void preOrderTraversal(Node* node, vector<string>& list) {
        if (node) {
            list.push_back(node->name);
            preOrderTraversal(node->left, list);
            preOrderTraversal(node->right, list);
        }
    }

    void postOrderTraversal(Node* node, vector<string>& list) {
        if (node) {
            postOrderTraversal(node->left, list);
            postOrderTraversal(node->right, list);
            list.push_back(node->name);
        }
    }

    // Helper functions for public methods
    void printTree(vector<string>& list) {
        for (long long unsigned int i = 0; i < list.size(); i++) {
            cout << list[i];
            if (i != list.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }

    void printID(vector<int>& list) {
        for (unsigned int i = 0; i < list.size(); i++) {
            cout << setw(8) << setfill('0') << list[i] << endl;
        }
    }

    // split search into search ID and name, they are called dependent on how the command is used
    void searchID(Node* node, int id, bool& found) {

        if(!node) {
            return;
        }

        if (id > node->id) {
            searchID(node->right, id, found);
        } else if (id < node->id) {
            searchID(node->left, id, found);
        } else {
            found = true;
            cout << node->name << endl;
        }

    }

    void searchName(Node* node, string& name, bool& found, vector<int>& list) {

        if(!node) {
            return;
        }

        if(node->name == name) {
            list.push_back(node->id);
            found = true;
        }
        searchName(node->left, name, found, list);
        searchName(node->right, name, found, list);


    }

    // function for destructor
    void deleteTree(Node* node) {
        if (!node) {
            return;
        }
        deleteTree(node->left);
        deleteTree(node->right);

        delete node;
    }

    // Verifies that commands include valid names and IDs,
    // Logic from cppreference: https://en.cppreference.com/w/cpp/string/byte/isdigit, https://en.cppreference.com/w/cpp/string/byte/isalpha
    bool verifyID(string id) {
        if (id.length() != 8) {
            return false;
        }

        for (char c : id) {
            if (!isdigit(c)) {
                return false;
            }
        }

        return true;
    }

    bool verifyName(const string& name) {

        for (char c : name) {
            if (!isalpha(c) && c != ' ') {
                return false;
            }
        }

        return true;
    }

public:
    void insert(string name, string id) {
        bool successfulInsert = false;

        if(!verifyName(name) || !verifyID(id)) {
            cout << "unsuccessful" << endl;
            return;
        } else {
            root = insertNode(root, name, stoi(id), successfulInsert);
        }

        if (successfulInsert) {
            cout << "successful" << endl;
        } else {
            cout << "unsuccessful" << endl;
        }
    }

    void deleteID(string id) {
        bool successfulDeletion = false;
        if(!verifyID(id)) {
            cout << "unsuccessful" << endl;
            return;
        } else {
            root = deleteNode(root, stoi(id), successfulDeletion);
        }

        if (successfulDeletion) {
            cout << "successful" << endl;
        } else {
            cout << "unsuccessful" << endl;
        }
    }

    void printInorder() {
        vector<string> printList;
        inOrderTraversal(root, printList);
        printTree(printList);

    }

    // for use in Test Case 5
    void printInorderIDs() {
        vector<string> printList;
        inOrderTraversalIDs(root, printList);
        printTree(printList);
    }

    void printPreorder() {
        vector<string> printList;
        preOrderTraversal(root, printList);
        printTree(printList);

    }

    void printPostorder() {
        vector<string> printList;
        postOrderTraversal(root, printList);
        printTree(printList);

    }

    void search(string name, int id) {
        bool found = false;
        // Main function passes "search by id" if the user runs the search command without an ID.
        if (name == "search by id") {
            searchID(root, id, found);
            if (!found) {
                cout << "unsuccessful" << endl;
            }

        } else {
            vector<int> idList;
            searchName(root, name, found, idList);
            printID(idList);

            if (!found) {
                cout << "unsuccessful" << endl;
                return;
            }
        }
    }

    void removeInorder(int n) {
        bool successfulDeletion = false;
        int index = -1;

        root = deleteNInOrder(root, n, successfulDeletion, index);

        if (successfulDeletion) {
            cout << "successful" << endl;
        } else {
            cout << "unsuccessful" << endl;
        }
    }

    void printLevelCount() {
        cout << height(root) << endl;
    }

    AVLTree() : root(nullptr) {}

    // Destructor
    ~AVLTree() {
        deleteTree(root);
    }
};




