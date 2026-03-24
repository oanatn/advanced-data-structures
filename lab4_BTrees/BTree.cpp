#include "BTree.h"

#include <iostream>
#include <queue>

BTree::BTree(int t) {
    this->root=nullptr;
    if (t<2) {
        std::cout<<"Invalid number. T must be at least 2. Therefore, t has been set to the default value of 2.\n";
        this->t=2;
    }
    else {
        this->t=t;
    }
}

void BTree::destroy(Node *node) {
    if (node==nullptr)
        return;
    if (node->leaf==false) {
        for (int i=0; i<node->children.size(); i++)
            destroy(node->children.at(i));
    }
        delete node;
}

BTree::~BTree() {
    destroy(root);
}

Node *BTree::find(Node *node, int key) const {
    if (node==nullptr)
        return nullptr;
    int i=0;
    while (i<node->keys.size() && key>node->keys[i]) {
        i++;
    }
    //now either key is greater than the values in this node, or key<=keys[i]
    if (i<node->keys.size() && node->keys[i]==key)
        return node; //found
    if (node->leaf==true)
        return nullptr;
    return find(node->children[i], key);
}

Node *BTree::search(int key) const {
    Node* x=find(root, key);
    if (x==nullptr)
        std::cout<<"The key was not found in the tree.\n";
    else
        std::cout<<"The given key was found!\n";
    return x;
}

bool BTree::empty() const {
    return root == nullptr;
}

int BTree::minimum() const {
    if (root == nullptr) {
        std::cout << "The tree is empty.\n";
        return 0;
    }

    Node* current = root;
    while (!current->leaf) {
        current = current->children[0];
    }
    return current->keys[0];
}

int BTree::maximum() const {
    if (root == nullptr) {
        std::cout << "The tree is empty.\n";
        return 0;
    }

    Node* current = root;
    while (!current->leaf) {
        current = current->children[current->children.size() - 1];
    }
    return current->keys[current->keys.size() - 1];
}

void BTree::insert(int key) {
    if (root == nullptr) {
        root = new Node(true);
        root->keys.push_back(key);
        return;
    }
    if (root->keys.size() == 2 * t - 1) {
        Node* newRoot = new Node(false);
        newRoot->children.push_back(root);
        splitChild(newRoot, 0);
        int i = 0;
        if (key > newRoot->keys[0]) {
            i = 1;
        }
        insertNonFull(newRoot->children[i], key);
        root = newRoot;
    } else {
        insertNonFull(root, key);
    }
}

void BTree::splitChild(Node* parent, int childIndex) {
    Node* fullChild = parent->children[childIndex];
    Node* newNode = new Node(fullChild->leaf);
    int middleKey = fullChild->keys[t - 1];
    // Move last t-1 keys from fullChild to newNode
    for (int i = t; i < fullChild->keys.size(); i++) {
        newNode->keys.push_back(fullChild->keys[i]);
    }
    // If not leaf, move last t children too
    if (!fullChild->leaf) {
        for (int i = t; i < fullChild->children.size(); i++) {
            newNode->children.push_back(fullChild->children[i]);
        }
    }
    // Shrink fullChild to keep only first t-1 keys
    fullChild->keys.resize(t - 1);
    // Shrink children if needed
    if (!fullChild->leaf) {
        fullChild->children.resize(t);
    }
    // Insert new child into parent
    parent->children.insert(parent->children.begin() + childIndex + 1, newNode);
    // Insert middle key into parent
    parent->keys.insert(parent->keys.begin() + childIndex, middleKey);
}

void BTree::insertNonFull(Node* node, int key) {
    int i = static_cast<int>(node->keys.size()) - 1;
    if (node->leaf) {
        node->keys.push_back(0); // make room
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
    } else {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;

        if (node->children[i]->keys.size() == 2 * t - 1) {
            splitChild(node, i);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

void BTree::remove(int key) {
    if (root == nullptr) {
        std::cout<<"The tree is empty.\n";
        return;
    }

    remove(root, key);

    if (root->keys.size() == 0) {
        Node* oldRoot = root;
        if (root->leaf) {
            root = nullptr;
        } else {
            root = root->children[0];
        }
        delete oldRoot;
    }
}

void BTree::remove(Node* node, int key) {
    int idx = 0;
    while (idx < node->keys.size() && node->keys[idx] < key) {
        idx++;
    }

    if (idx < node->keys.size() && node->keys[idx] == key) {
        if (node->leaf) {
            removeFromLeaf(node, idx);
        } else {
            removeFromNonLeaf(node, idx);
        }
    } else {
        if (node->leaf) {
            std::cout<<"The key "<<key<<" does not exist in the tree.\n";
            return;
        }

        bool flag = (idx == node->keys.size());

        if (node->children[idx]->keys.size() < t) {
            fill(node, idx);
        }

        if (flag && idx > node->keys.size()) {
            remove(node->children[idx - 1], key);
        } else {
            remove(node->children[idx], key);
        }
    }
}

void BTree::removeFromLeaf(Node* node, int index) {
    node->keys.erase(node->keys.begin() + index);
}

void BTree::removeFromNonLeaf(Node* node, int index) {
    int key = node->keys[index];

    if (node->children[index]->keys.size() >= t) {
        int pred = getPredecessor(node, index);
        node->keys[index] = pred;
        remove(node->children[index], pred);
    }
    else if (node->children[index + 1]->keys.size() >= t) {
        int succ = getSuccessor(node, index);
        node->keys[index] = succ;
        remove(node->children[index + 1], succ);
    }
    else {
        merge(node, index);
        remove(node->children[index], key);
    }
}

int BTree::getPredecessor(Node* node, int index) const {
    Node* current = node->children[index];
    while (!current->leaf) {
        current = current->children[current->keys.size()];
    }
    return current->keys[current->keys.size() - 1];
}

int BTree::getSuccessor(Node* node, int index) const {
    Node* current = node->children[index + 1];
    while (!current->leaf) {
        current = current->children[0];
    }
    return current->keys[0];
}

void BTree::fill(Node* node, int index) {
    if (index != 0 && node->children[index - 1]->keys.size() >= t) {
        borrowFromPrev(node, index);
    }
    else if (index != node->keys.size() && node->children[index + 1]->keys.size() >= t) {
        borrowFromNext(node, index);
    }
    else {
        if (index != node->keys.size()) {
            merge(node, index);
        } else {
            merge(node, index - 1);
        }
    }
}

void BTree::borrowFromPrev(Node* node, int index) {
    Node* child = node->children[index];
    Node* sibling = node->children[index - 1];

    child->keys.insert(child->keys.begin(), node->keys[index - 1]);

    if (!child->leaf) {
        child->children.insert(child->children.begin(), sibling->children[sibling->children.size() - 1]);
        sibling->children.pop_back();
    }

    node->keys[index - 1] = sibling->keys[sibling->keys.size() - 1];
    sibling->keys.pop_back();
}

void BTree::borrowFromNext(Node* node, int index) {
    Node* child = node->children[index];
    Node* sibling = node->children[index + 1];

    child->keys.push_back(node->keys[index]);

    if (!child->leaf) {
        child->children.push_back(sibling->children[0]);
        sibling->children.erase(sibling->children.begin());
    }

    node->keys[index] = sibling->keys[0];
    sibling->keys.erase(sibling->keys.begin());
}

void BTree::merge(Node* node, int index) {
    Node* child = node->children[index];
    Node* sibling = node->children[index + 1];

    child->keys.push_back(node->keys[index]);

    for (int i = 0; i < sibling->keys.size(); i++) {
        child->keys.push_back(sibling->keys[i]);
    }

    if (!child->leaf) {
        for (int i = 0; i < sibling->children.size(); i++) {
            child->children.push_back(sibling->children[i]);
        }
    }
    node->keys.erase(node->keys.begin() + index);
    node->children.erase(node->children.begin() + index + 1);
    delete sibling;
}

void BTree::printTree() const {
    if (root == nullptr) {
        std::cout << "The tree is empty.\n";
        return;
    }

    std::queue<Node*> q;
    q.push(root);

    int level = 0;

    while (!q.empty()) {
        int levelSize = q.size();
        std::cout << "Level " << level << ": ";

        for (int i = 0; i < levelSize; i++) {
            Node* current = q.front();
            q.pop();

            std::cout << "[";
            for (int j = 0; j < current->keys.size(); j++) {
                std::cout << current->keys[j];
                if (j < current->keys.size() - 1) {
                    std::cout << " ";
                }
            }
            std::cout << "] ";

            if (!current->leaf) {
                for (int j = 0; j < current->children.size(); j++) {
                    q.push(current->children[j]);
                }
            }
        }

        std::cout << "\n";
        level++;
    }
}