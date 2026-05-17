#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H

#include "OperationLogger.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

struct HeapNode {
    int key;
    int matchId;
    int degree;
    HeapNode* parent;
    HeapNode* child;
    HeapNode* sibling;

    explicit HeapNode(int value, int matchId = -1)
        : key(value), matchId(matchId), degree(0), parent(nullptr),
          child(nullptr), sibling(nullptr) {}
};

class BinomialHeap {
private:
    HeapNode* head;
    OperationLogger* logger;

    void binomialLink(HeapNode* y, HeapNode* z);
    HeapNode* mergeRootLists(HeapNode* h1, HeapNode* h2);
    HeapNode* unionHeaps(HeapNode* h1, HeapNode* h2);
    static HeapNode* reverseChildren(HeapNode* node);
    static void destroyTree(HeapNode* node);
    static HeapNode* findMinNode(HeapNode* head);
    static void printTree(HeapNode* root, int depth);

public:
    BinomialHeap();
    ~BinomialHeap();

    BinomialHeap(const BinomialHeap&) = delete;
    BinomialHeap& operator=(const BinomialHeap&) = delete;

    BinomialHeap(BinomialHeap&& other) noexcept;
    BinomialHeap& operator=(BinomialHeap&& other) noexcept;

    void setLogger(OperationLogger* logger);

    bool empty() const;
    void clear();
    HeapNode* insert(int key, int matchId = -1);
    HeapNode* getMin() const;
    HeapNode* extractMin();
    void merge(BinomialHeap& other);
    void decreaseKey(HeapNode* x, int newKey);
    void deleteNode(HeapNode* x);
    void print() const;
};

#endif