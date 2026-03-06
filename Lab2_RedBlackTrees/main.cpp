#include <iostream>
#include "RbTree.h"

void initialInsert(RbTree& tree) {
    unsigned int v[100];
    unsigned int x;
    int i = 0;

    std::cout << "Please enter the keys you would like to insert into the tree. Type 0 to stop inserting new numbers.\n";
    std::cin >> x;

    if (x == 0) {
        std::cout << "Error! You must provide at least one number.\n";
        return;
    }

    while (x != 0 && i < 100) {
        v[i++] = x;
        std::cin >> x;
    }

    for (int k = 0; k < i; k++)
        tree.insert(v[k]);

    tree.showTree();
}

void showMenu() {
    std::cout << "\nChoose an option:\n";
    std::cout << "1. Insert a new node\n";
    std::cout << "2. Remove a node\n";
    std::cout << "3. Show the minimum\n";
    std::cout << "4. Show the maximum\n";
    std::cout << "5. Search for a value\n";
    std::cout << "0. Exit\n";
}

void runMenu(RbTree& tree) {
    int option;
    int value;
    Node* result;

    do {
        showMenu();
        std::cin >> option;

        switch (option) {
            case 1:
                std::cout << "Value to insert: ";
                std::cin >> value;
                tree.insert(value);
                tree.showTree();
                break;

            case 2:
                std::cout << "Value to remove: ";
                std::cin >> value;
                tree.deleteNode(value);
                tree.showTree();
                break;

            case 3:
                result = tree.minimum();
                if (result != tree.search(result->key))
                    std::cout << "Tree is empty.\n";
                else
                    std::cout << "Minimum value: " << result->key << "\n";
                break;

            case 4:
                result = tree.maximum();
                if (result != tree.search(result->key))
                    std::cout << "Tree is empty.\n";
                else
                    std::cout << "Maximum value: " << result->key << "\n";
                break;

            case 5:
                std::cout << "Value to search: ";
                std::cin >> value;
                result = tree.search(value);
                if (result->key == value)
                    std::cout << "Value found in tree.\n";
                else
                    std::cout << "Value not found in tree.\n";
                break;

            case 0:
                std::cout << "Exiting.\n";
                break;

            default:
                std::cout << "Invalid option.\n";
        }
    } while (option != 0);
}

int main() {
    RbTree tree;
    initialInsert(tree);
    runMenu(tree);
    return 0;
}