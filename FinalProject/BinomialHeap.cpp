#include "BinomialHeap.h"

BinomialHeap::BinomialHeap() : head(nullptr), logger(nullptr) {
}

BinomialHeap::~BinomialHeap() {
    clear();
}

BinomialHeap::BinomialHeap(BinomialHeap&& other) noexcept
    : head(other.head), logger(other.logger) {
    other.head = nullptr;
    other.logger = nullptr;
}

BinomialHeap& BinomialHeap::operator=(BinomialHeap&& other) noexcept {
    if (this != &other) {
        clear();
        head = other.head;
        logger = other.logger;
        other.head = nullptr;
        other.logger = nullptr;
    }
    return *this;
}

void BinomialHeap::setLogger(OperationLogger* logger) {
    this->logger = logger;
}

bool BinomialHeap::empty() const {
    return head == nullptr;
}

void BinomialHeap::clear() {
    destroyTree(head);
    head = nullptr;
}

void BinomialHeap::binomialLink(HeapNode* y, HeapNode* z) {
    if (logger) {
        logger->log(
            "Binomial heap: linking tree with root priority "
            + std::to_string(y->key)
            + " under root priority "
            + std::to_string(z->key)
            + "."
        );
    }

    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree++;
}

HeapNode* BinomialHeap::mergeRootLists(HeapNode* h1, HeapNode* h2) {
    if (logger) {
        logger->log("Binomial heap: merging root lists ordered by degree.");
    }

    if (!h1) {
        return h2;
    }
    if (!h2) {
        return h1;
    }

    HeapNode* headMerged = nullptr;
    HeapNode* tail = nullptr;

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

HeapNode* BinomialHeap::unionHeaps(HeapNode* h1, HeapNode* h2) {
    if (logger) {
        logger->log("Binomial heap: union operation started.");
    }

    HeapNode* newHead = mergeRootLists(h1, h2);

    if (!newHead) {
        if (logger) {
            logger->log("Binomial heap: both heaps were empty.");
        }

        return nullptr;
    }

    HeapNode* prev = nullptr;
    HeapNode* curr = newHead;
    HeapNode* next = curr->sibling;

    while (next) {
        if (curr->degree != next->degree ||
            (next->sibling && next->sibling->degree == curr->degree)) {
            prev = curr;
            curr = next;
        } else if (curr->key <= next->key) {
            if (logger) {
                logger->log(
                    "Binomial heap: two trees of degree "
                    + std::to_string(curr->degree)
                    + " found; smaller root stays on top."
                );
            }

            curr->sibling = next->sibling;
            binomialLink(next, curr);
        } else {
            if (logger) {
                logger->log(
                    "Binomial heap: two trees of degree "
                    + std::to_string(curr->degree)
                    + " found; next root becomes parent."
                );
            }

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

    if (logger) {
        logger->log("Binomial heap: union operation complete.");
    }

    return newHead;
}

HeapNode* BinomialHeap::reverseChildren(HeapNode* node) {
    HeapNode* prev = nullptr;
    HeapNode* curr = node;

    while (curr) {
        HeapNode* next = curr->sibling;
        curr->sibling = prev;
        curr->parent = nullptr;
        prev = curr;
        curr = next;
    }

    return prev;
}

void BinomialHeap::destroyTree(HeapNode* node) {
    while (node) {
        HeapNode* next = node->sibling;
        destroyTree(node->child);
        delete node;
        node = next;
    }
}

HeapNode* BinomialHeap::findMinNode(HeapNode* head) {
    if (!head) {
        return nullptr;
    }

    HeapNode* minNode = head;
    HeapNode* curr = head->sibling;

    while (curr) {
        if (curr->key < minNode->key) {
            minNode = curr;
        }

        curr = curr->sibling;
    }

    return minNode;
}

HeapNode* BinomialHeap::insert(int key, int matchId) {
    if (logger) {
        logger->log(
            "Binomial heap: inserting match #"
            + std::to_string(matchId)
            + " with priority "
            + std::to_string(key)
            + "."
        );
    }

    BinomialHeap temp;
    temp.setLogger(logger);

    HeapNode* newNode = new HeapNode(key, matchId);
    temp.head = newNode;

    head = unionHeaps(head, temp.head);
    temp.head = nullptr;

    if (logger) {
        logger->log("Binomial heap: insertion complete.");
    }

    return newNode;
}

HeapNode* BinomialHeap::getMin() const {
    return findMinNode(head);
}

HeapNode* BinomialHeap::extractMin() {
    if (logger) {
        logger->log("Binomial heap: extracting minimum-priority match.");
    }

    if (!head) {
        if (logger) {
            logger->log("Binomial heap: heap is empty, nothing to extract.");
        }

        return nullptr;
    }

    HeapNode* minNode = head;
    HeapNode* minPrev = nullptr;
    HeapNode* prev = nullptr;
    HeapNode* curr = head;

    while (curr) {
        if (logger) {
            logger->log(
                "Binomial heap: checking root with priority "
                + std::to_string(curr->key)
                + "."
            );
        }

        if (curr->key < minNode->key) {
            minNode = curr;
            minPrev = prev;

            if (logger) {
                logger->log(
                    "Binomial heap: new minimum found with priority "
                    + std::to_string(minNode->key)
                    + "."
                );
            }
        }

        prev = curr;
        curr = curr->sibling;
    }

    if (logger) {
        logger->log(
            "Binomial heap: removing root with minimum priority "
            + std::to_string(minNode->key)
            + "."
        );
    }

    if (minPrev) {
        minPrev->sibling = minNode->sibling;
    } else {
        head = minNode->sibling;
    }

    if (logger) {
        logger->log("Binomial heap: reversing extracted root's children.");
    }

    HeapNode* childReversed = reverseChildren(minNode->child);

    minNode->child = nullptr;
    minNode->sibling = nullptr;
    minNode->parent = nullptr;

    if (logger) {
        logger->log("Binomial heap: merging children back into the heap.");
    }

    head = unionHeaps(head, childReversed);

    if (logger) {
        logger->log("Binomial heap: extract-min operation complete.");
    }

    return minNode;
}

void BinomialHeap::merge(BinomialHeap& other) {
    if (this == &other) {
        return;
    }

    if (logger) {
        logger->log("Binomial heap: merging two heaps.");
    }

    head = unionHeaps(head, other.head);
    other.head = nullptr;

    if (logger) {
        logger->log("Binomial heap: merge complete.");
    }
}

void BinomialHeap::decreaseKey(HeapNode* x, int newKey) {
    if (!x) {
        throw std::invalid_argument("Null node pointer");
    }

    if (newKey > x->key) {
        throw std::invalid_argument("New key is greater than current key");
    }

    if (logger) {
        logger->log(
            "Binomial heap: decreasing key from "
            + std::to_string(x->key)
            + " to "
            + std::to_string(newKey)
            + "."
        );
    }

    x->key = newKey;

    HeapNode* y = x;
    HeapNode* z = y->parent;

    while (z && y->key < z->key) {
        if (logger) {
            logger->log("Binomial heap: child priority is smaller than parent, swapping keys.");
        }

        std::swap(y->key, z->key);
        std::swap(y->matchId, z->matchId);

        y = z;
        z = y->parent;
    }

    if (logger) {
        logger->log("Binomial heap: decrease-key operation complete.");
    }
}

void BinomialHeap::deleteNode(HeapNode* x) {
    if (!x) {
        throw std::invalid_argument("Null node pointer");
    }

    if (logger) {
        logger->log("Binomial heap: deleting node by decreasing it to negative infinity.");
    }

    decreaseKey(x, std::numeric_limits<int>::min());

    HeapNode* minNode = extractMin();
    delete minNode;

    if (logger) {
        logger->log("Binomial heap: delete-node operation complete.");
    }
}

void BinomialHeap::printTree(HeapNode* root, int depth) {
    if (!root) {
        return;
    }

    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }

    std::cout << "priority=" << root->key;

    if (root->matchId != -1) {
        std::cout << ", match #" << root->matchId;
    }

    std::cout << '\n';

    HeapNode* child = root->child;

    while (child) {
        printTree(child, depth + 1);
        child = child->sibling;
    }
}

void BinomialHeap::print() const {
    if (!head) {
        std::cout << "Heap is empty\n";
        return;
    }

    HeapNode* curr = head;

    while (curr) {
        std::cout << "B" << curr->degree << ":\n";
        printTree(curr, 0);
        curr = curr->sibling;
        std::cout << '\n';
    }
}