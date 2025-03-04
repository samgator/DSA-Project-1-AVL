/*#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "AVL_Tree.h"
#include <algorithm>

using namespace std;

// Logic for stream buffer from given link: https://stackoverflow.com/questions/4191089/how-to-unit-test-function-writing-to-stdout-stdcout
TEST_CASE("Incorrect Commands") {
    AVLTree tree;
    ostringstream oss;
    streambuf* testCout = cout.rdbuf();
    cout.rdbuf(oss.rdbuf());

    // Name contains numbers
    tree.insert("A11y", "12345678");
    REQUIRE(oss.str() == "unsuccessful\n");
    oss.str("");

    // Invalid ID
    tree.insert("Sam", "-1");
    REQUIRE(oss.str() == "unsuccessful\n");
    oss.str("");

    // Deletion of invalid ID
    tree.deleteID("1");
    REQUIRE(oss.str() == "unsuccessful\n");
    oss.str("");

    // Searching for invalid ID
    tree.search("search by id", 1);
    REQUIRE(oss.str() == "unsuccessful\n");
    oss.str("");

    // Searching for invalid Name
    tree.search("\"B0b\"", 12345678);
    REQUIRE(oss.str() == "unsuccessful\n");
    oss.str("");

    // Ensuring tree is empty
    tree.printInorder();
    REQUIRE(oss.str() == "\n");

    cout.rdbuf(testCout);

}

TEST_CASE("Edge Cases") {
    AVLTree tree;
    ostringstream oss;
    streambuf* testCout = cout.rdbuf(oss.rdbuf());

    // Deletion from empty tree
    tree.deleteID("0");
    REQUIRE(oss.str() == "unsuccessful\n");
    oss.str("");

    // Printing level count of empty tree
    tree.printLevelCount();
    REQUIRE(oss.str() == "0\n");
    oss.str("");

    // Inorder deletion of empty tree
    tree.removeInorder(0);
    REQUIRE(oss.str() == "unsuccessful\n");

    cout.rdbuf(testCout);
}


TEST_CASE("Rotation Case") {
    AVLTree tree;
    ostringstream oss;
    streambuf* testCout = cout.rdbuf(oss.rdbuf());

    //Left Left Case
    tree.insert("D", "00000004");
    tree.insert("C", "00000003");
    tree.insert("B", "00000002");
    tree.insert("A", "00000001");
    tree.printInorder();
    REQUIRE(oss.str() == "successful\nsuccessful\nsuccessful"
                         "\nsuccessful\nA, B, C, D\n");
    oss.str("");

    AVLTree tree2;

    //Right Right Case
    tree2.insert("A", "00000001");
    tree2.insert("B", "00000002");
    tree2.insert("C", "00000003");
    tree2.insert("D", "00000004");
    tree2.printInorder();
    REQUIRE(oss.str() == "successful\nsuccessful\nsuccessful"
                         "\nsuccessful\nA, B, C, D\n");
    oss.str("");

    AVLTree tree3;

    //Right Left Case
    tree3.insert("A", "00000001");
    tree3.insert("C", "00000003");
    tree3.insert("B", "00000002");
    tree3.insert("D", "00000004");
    tree3.printInorder();
    REQUIRE(oss.str() == "successful\nsuccessful\nsuccessful\n"
                         "successful\nA, B, C, D\n");
    oss.str("");

    AVLTree tree4;

    //Left Right Case
    tree4.insert("C", "00000003");
    tree4.insert("A", "00000001");
    tree4.insert("B", "00000002");
    tree4.insert("D", "00000004");
    tree4.printInorder();
    REQUIRE(oss.str() == "successful\nsuccessful\nsuccessful\n"
                         "successful\nA, B, C, D\n");
    oss.str("");

    cout.rdbuf(testCout);
}

TEST_CASE("Deletion") {
    AVLTree tree;
    ostringstream oss;
    streambuf* testCout = cout.rdbuf(oss.rdbuf());

    //Node has zero children
    tree.insert("A", "00000001");
    tree.printInorder();
    tree.deleteID("00000001");
    tree.printInorder();
    REQUIRE(oss.str() == "successful\nA\nsuccessful\n\n");
    oss.str("");

    //Node has one child
    tree.insert("A", "00000001");
    tree.insert("B", "00000002");
    tree.printInorder();
    tree.deleteID("00000001");
    tree.printInorder();
    REQUIRE(oss.str() == "successful\nsuccessful\nA, B\nsuccessful\nB\n");
    tree.deleteID("00000002");
    oss.str("");

    //Node has two children
    tree.insert("A", "00000001");
    tree.insert("B", "00000002");
    tree.insert("C", "00000003");
    tree.printInorder();
    tree.deleteID("00000002");
    tree.printInorder();
    REQUIRE(oss.str() == "successful\nsuccessful\nsuccessful\n"
                         "A, B, C\nsuccessful\nA, C\n");

    cout.rdbuf(testCout);
}

// Referenced https://www.onlinegdb.com/Ih4XEhD80w for this test
TEST_CASE("Inserting 100 Nodes and Deleting 10") {
    AVLTree tree;
    ostringstream oss;
    streambuf* testCout = cout.rdbuf(oss.rdbuf());

    vector<string> actualOutput, expectedOutput;
    string expectedOutputString;

    // Insert 100 nodes
    for (int i = 1; i <= 100; i++) {

        // Ensure inserted ID is valid
        ostringstream idStream;
        idStream << setw(8) << setfill('0') << i;
        string id = idStream.str();

        tree.insert("person", id);
        expectedOutput.push_back(to_string(i));

        // Verify successful insertions
        REQUIRE(oss.str() == "successful\n");
        oss.str("");
    }

    // 10 random deletions
    int uniqueDeletions = 0;
    vector<string> deletedIDs;
    vector<string>::iterator it;

    while (uniqueDeletions < 10) {
        int random = rand() % 100;
        // Referenced https://en.cppreference.com/w/cpp/algorithm/find for find() command.
        // Referenced https://en.cppreference.com/w/cpp/container/vector/erase for erase() command.
        if(!(find(expectedOutput.begin(), expectedOutput.end(), to_string(random)) == expectedOutput.end())) {
            it = find(expectedOutput.begin(), expectedOutput.end(), to_string(random));
            expectedOutput.erase(it);

            // Ensure deleted ID is valid
            ostringstream idStream;
            idStream << setw(8) << setfill('0') << random;
            string id = idStream.str();
            tree.deleteID(id);

            // Verify successful deletions
            REQUIRE(oss.str() == "successful\n");
            oss.str("");

            deletedIDs.push_back(to_string(random));
            uniqueDeletions++;
        }
    }

    tree.printInorderIDs();

    istringstream iss(oss.str());
    string line;

    // Convert output from inorder traversal to a vector
    while (getline(iss, line, ',')) {
        // Referenced https://cplusplus.com/reference/istream/istream/ignore/ for iss.ignore()
        iss.ignore();
        actualOutput.push_back(line);
    }

    // append newline character to end of expected output to match AVLTree function call
    expectedOutput[expectedOutput.size() - 1] += "\n";

    REQUIRE(actualOutput == expectedOutput);

    cout.rdbuf(testCout);

}
*/

