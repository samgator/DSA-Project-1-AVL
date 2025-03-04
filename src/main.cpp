#include <iostream>
#include "AVL_Tree.h"
#include <sstream>
#include <cctype>
#include <limits>

using namespace std;

// Verifies that commands include valid IDs,
// Logic from cppreference: https://en.cppreference.com/w/cpp/string/byte/isdigit
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


int main() {
    // Initialize tree and read first input: number of commands
    AVLTree tree;
    string command;
    int numCommands;
    string name, input;
    string id;
    int N;

    cin >> numCommands;


    int n = 0;

    while (n < numCommands) {
        cin >> command;

        // Referenced https://cplusplus.com/reference/istream/istream/ignore/ for cin.ignore()
        if (command == "insert") {
            // ignore the space after command title
            cin.ignore();
            getline(cin, name, '"');
            getline(cin, name, '"');
            cin >> id;


            tree.insert(name,id);
            
        } else if (command == "remove") {
            cin.ignore();
            cin >> id;

            tree.deleteID(id);

        } else if (command == "search") {
            cin.ignore();
            getline(cin, input);

            // If a quotation mark is entered, search by name
            if (input[0] == '"') {
                stringstream in(input);
                getline(in, name, '"');
                getline(in, name, '"');

                tree.search(name, 0);
            } else {
                // if no quotation mark is detected, search by ID by passing name as "search by id"
                id = input;
                if(!verifyID(id)) {
                    cout << "unsuccessful" << endl;
                } else {
                    tree.search("search by id", stoi(id));
                }


            }
        // traversals
        } else if (command == "printInorder") {
            tree.printInorder();
        } else if (command == "printPreorder") {
            tree.printPreorder();
        } else if (command == "printPostorder") {
            tree.printPostorder();
        } else if (command == "printLevelCount") {
           tree.printLevelCount();
        } else if (command == "removeInorder") {
            cin.ignore();
            cin >> N;

            tree.removeInorder(N);
        } else {
            cout << "unsuccessful" << endl;
        }

        n++;
    }

    return 0;
}
