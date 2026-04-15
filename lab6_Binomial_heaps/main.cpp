#include <iostream>
#include "BinomialHeap.h"

void initialInsert(BinomialHeap& heap) {
    int v[100];
    int x;
    int i = 0;
    std::cout << "Please enter the keys you would like to insert into the heap. Type 0 to stop inserting new numbers.\n";
    std::cin >> x;
    if (x == 0) {
        std::cout << "Error! You must provide at least one number.\n";
        return;
    }
    while (x != 0 && i < 100) {
        v[i++] = x;
        std::cin >> x;
    }
    for (int k = 0; k < i; k++) {
        heap.insert(v[k]);
    }
    heap.print();
}

void buildSecondHeap(BinomialHeap& heap) {
    int v[100];
    int x;
    int i = 0;
    std::cout << "Enter the keys for the second heap. Type 0 to stop.\n";
    std::cin >> x;
    if (x == 0) {
        std::cout << "Second heap remains empty.\n";
        return;
    }
    while (x != 0 && i < 100) {
        v[i++] = x;
        std::cin >> x;
    }
    for (int k = 0; k < i; k++) {
        heap.insert(v[k]);
    }
}

void showMenu() {
    std::cout << "\nChoose an option:\n";
    std::cout << "1. Insert a new key\n";
    std::cout << "2. Show the minimum\n";
    std::cout << "3. Extract the minimum\n";
    std::cout << "4. Merge with another heap\n";
    std::cout << "5. Print the heap\n";
    std::cout << "6. Check if the heap is empty\n";
    std::cout << "7. Clear the heap\n";
    std::cout << "0. Exit\n";
}

void runMenu(BinomialHeap& heap) {
    int option;
    int value;
    Node* result;
    BinomialHeap other;
    do {
        showMenu();
        std::cin >> option;
        switch (option) {
            case 1:
                std::cout << "Value to insert: ";
                std::cin >> value;
                heap.insert(value);
                heap.print();
                break;
            case 2:
                if (heap.empty()) {
                    std::cout << "Heap is empty.\n";
                } else {
                    result = heap.getMin();
                    std::cout << "Minimum value: " << result->key << "\n";
                }
                break;
            case 3:
                if (heap.empty()) {
                    std::cout << "Heap is empty.\n";
                } else {
                    result = heap.extractMin();
                    std::cout << "Extracted minimum value: " << result->key << "\n";
                    delete result;
                    heap.print();
                }
                break;
            case 4:
                other.clear();
                buildSecondHeap(other);
                heap.merge(other);
                std::cout << "Heaps merged.\n";
                heap.print();
                break;
            case 5:
                heap.print();
                break;
            case 6:
                if (heap.empty()) {
                    std::cout << "Heap is empty.\n";
                } else {
                    std::cout << "Heap is not empty.\n";
                }
                break;
            case 7:
                heap.clear();
                std::cout << "Heap cleared.\n";
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
    BinomialHeap heap;
    initialInsert(heap);
    runMenu(heap);
    return 0;
}