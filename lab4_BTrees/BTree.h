#ifndef BTREE_H
#define BTREE_H

#include <vector>
struct Node {
    std::vector<int> keys;
    std::vector<Node*> children;
    bool leaf;

    Node(bool isLeaf) : leaf(isLeaf) {}
};


class BTree {
private:
    Node* root;
    int t;

    // Search helpers
    Node* find(Node* node, int key) const;

    // Insertion helpers
    void splitChild(Node* parent, int childIndex);
    void insertNonFull(Node* node, int key);

    // Deletion helpers
    void remove(Node* node, int key);
    void removeFromLeaf(Node* node, int index);
    void removeFromNonLeaf(Node* node, int index);
    int getPredecessor(Node* node, int index) const;
    int getSuccessor(Node* node, int index) const;
    void fill(Node* node, int index);
    void borrowFromPrev(Node* node, int index);
    void borrowFromNext(Node* node, int index);
    void merge(Node* node, int index);

    // Cleanup helper
    void destroy(Node* node);

public:
    explicit BTree(int t);
    ~BTree();
    Node* search(int key) const;
    void insert(int key);
    void remove(int key);
    bool empty() const;
    int minimum() const;
    int maximum() const;
    void printTree() const;
};

#endif // BTREE_H