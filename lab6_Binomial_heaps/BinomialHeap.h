#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H

#include <iostream>
#include <limits>

struct Node {
    int key;
    int degree;         // Number of children
    Node* parent;
    Node* child;
    Node* sibling;      // Pointer to right sibling

    explicit Node(int value)
        : key(value), degree(0), parent(nullptr),
          child(nullptr), sibling(nullptr) {}
};

class BinomialHeap {
private:
    Node* head;  // Pointer to the first root in the root list
    static void binomialLink(Node* y, Node* z);
    static Node* mergeRootLists(Node* h1, Node* h2);
    static Node* unionHeaps(Node* h1, Node* h2);
    static Node* reverseChildren(Node* node);
    static void destroyTree(Node* node);
    static Node* findMinNode(Node* head);
    static void printTree(Node* root, int depth);

public:
    BinomialHeap();
    ~BinomialHeap();
    // Disable copying to prevent shallow copies
    BinomialHeap(const BinomialHeap&) = delete;
    BinomialHeap& operator=(const BinomialHeap&) = delete;

    // Enable move semantics
    BinomialHeap(BinomialHeap&& other) noexcept;
    BinomialHeap& operator=(BinomialHeap&& other) noexcept;

    bool empty() const;
    void clear();
    Node* insert(int key);
    Node* getMin() const;
    Node* extractMin();             // Removes and returns the minimum node
    void merge(BinomialHeap& other);
    void decreaseKey(Node* x, int newKey);
    void deleteNode(Node* x);
    void print() const;
};

#endif // BINOMIAL_HEAP_H