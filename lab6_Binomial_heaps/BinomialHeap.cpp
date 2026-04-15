#include "BinomialHeap.h"

BinomialHeap::BinomialHeap() : head(nullptr) {
}

BinomialHeap::~BinomialHeap() {
    clear();
}

BinomialHeap::BinomialHeap(BinomialHeap&& other) noexcept : head(other.head) {
    other.head = nullptr;
}

BinomialHeap& BinomialHeap::operator=(BinomialHeap&& other) noexcept {
    if (this != &other) {
        clear();
        head = other.head;
        other.head = nullptr;
    }
    return *this;
}

bool BinomialHeap::empty() const {
    return head == nullptr;
}

void BinomialHeap::clear() {
    destroyTree(head);
    head = nullptr;
}

void BinomialHeap::binomialLink(Node* y, Node* z) {
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree++;
}

Node* BinomialHeap::mergeRootLists(Node* h1, Node* h2) {
    if (!h1) {
        return h2;
    }
    if (!h2) {
        return h1;
    }
    Node* headMerged = nullptr;
    Node* tail = nullptr;
    if (h1->degree <= h2->degree) {
        headMerged = h1;
        h1 = h1->sibling;
    } else {
        headMerged = h2;
        h2 = h2->sibling;
    }
    tail = headMerged;
    while (h1 && h2) {
        if (h1->degree <= h2->degree) {
            tail->sibling = h1;
            h1 = h1->sibling;
        } else {
            tail->sibling = h2;
            h2 = h2->sibling;
        }
        tail = tail->sibling;
    }
    if (h1) {
        tail->sibling = h1;
    } else {
        tail->sibling = h2;
    }
    return headMerged;
}

Node* BinomialHeap::unionHeaps(Node* h1, Node* h2) {
    Node* newHead = mergeRootLists(h1, h2);
    if (!newHead) {
        return nullptr;
    }
    Node* prev = nullptr;
    Node* curr = newHead;
    Node* next = curr->sibling;
    while (next) {
        if (curr->degree != next->degree || (next->sibling && next->sibling->degree == curr->degree)) {
            prev = curr;
            curr = next;
        } else if (curr->key <= next->key) {
            curr->sibling = next->sibling;
            binomialLink(next, curr);
        } else {
            if (!prev) {
                newHead = next;
            } else {
                prev->sibling = next;
            }
            binomialLink(curr, next);
            curr = next;
        }
        next = curr->sibling;
    }
    return newHead;
}

Node* BinomialHeap::reverseChildren(Node* node) {
    Node* prev = nullptr;
    Node* curr = node;
    while (curr) {
        Node* next = curr->sibling;
        curr->sibling = prev;
        curr->parent = nullptr;
        prev = curr;
        curr = next;
    }
    return prev;
}

void BinomialHeap::destroyTree(Node* node) {
    while (node) {
        Node* next = node->sibling;
        destroyTree(node->child);
        delete node;
        node = next;
    }
}

Node* BinomialHeap::findMinNode(Node* head) {
    if (!head) {
        return nullptr;
    }
    Node* minNode = head;
    Node* curr = head->sibling;
    while (curr) {
        if (curr->key < minNode->key) {
            minNode = curr;
        }
        curr = curr->sibling;
    }
    return minNode;
}

Node* BinomialHeap::insert(int key) {
    BinomialHeap temp;
    Node* newNode = new Node(key);
    temp.head = newNode;
    head = unionHeaps(head, temp.head);
    temp.head = nullptr;
    return newNode;
}

Node* BinomialHeap::getMin() const {
    return findMinNode(head);
}

Node* BinomialHeap::extractMin() {
    if (!head) {
        return nullptr;
    }
    Node* minNode = head;
    Node* minPrev = nullptr;
    Node* prev = nullptr;
    Node* curr = head;
    while (curr) {
        if (curr->key < minNode->key) {
            minNode = curr;
            minPrev = prev;
        }
        prev = curr;
        curr = curr->sibling;
    }
    if (minPrev) {
        minPrev->sibling = minNode->sibling;
    } else {
        head = minNode->sibling;
    }
    Node* childReversed = reverseChildren(minNode->child);
    minNode->child = nullptr;
    minNode->sibling = nullptr;
    minNode->parent = nullptr;
    head = unionHeaps(head, childReversed);
    return minNode;
}

void BinomialHeap::merge(BinomialHeap& other) {
    if (this == &other) {
        return;
    }
    head = unionHeaps(head, other.head);
    other.head = nullptr;
}

void BinomialHeap::decreaseKey(Node* x, int newKey) {
    if (!x) {
        throw std::invalid_argument("Null node pointer");
    }
    if (newKey > x->key) {
        throw std::invalid_argument("New key is greater than current key");
    }
    x->key = newKey;
    Node* y = x;
    Node* z = y->parent;
    while (z && y->key < z->key) {
        std::swap(y->key, z->key);
        y = z;
        z = y->parent;
    }
}

void BinomialHeap::deleteNode(Node* x) {
    if (!x) {
        throw std::invalid_argument("Null node pointer");
    }
    decreaseKey(x, std::numeric_limits<int>::min());
    Node* minNode = extractMin();
    delete minNode;
}

void BinomialHeap::printTree(Node* root, int depth) {
    while (root) {
        for (int i = 0; i < depth; ++i) {
            std::cout << "  ";
        }
        std::cout << root->key << '\n';
        printTree(root->child, depth + 1);
        root = root->sibling;
    }
}

void BinomialHeap::print() const {
    if (!head) {
        std::cout << "Heap is empty\n";
        return;
    }
    Node* curr = head;
    while (curr) {
        std::cout << "B" << curr->degree << ":\n";
        printTree(curr, 0);
        curr = curr->sibling;
        std::cout << '\n';
    }
}