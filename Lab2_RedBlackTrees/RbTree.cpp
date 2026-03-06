#include"RbTree.h"

#include <iostream>

RbTree::RbTree() {
    this->nil=new Node;
    this->nil->color=BLACK;
    this->nil->left=this->nil->right=nil;
    this->nil->parent=nil;
    this->nil->key=-1;
    this->root=nil;
}

Node *RbTree::minOnBranch(Node *x) const {
    Node* min=x;
    while (min->left!=nil) {
        min=min->left;
    }
    return min;
}

Node *RbTree::maxOnBranch(Node *x) const {
    Node* max=x;
    while ((max->right!=nil)) {
        max=max->right;
    }
    return max;
}

Node *RbTree::minimum() const {
if (root==nil)
    return nil;
    else
        return minOnBranch(root);
}

Node *RbTree::maximum() const {
    if (root==nil)
        return nil;
    else
        return maxOnBranch(root);
}

Node *RbTree::search(int key) const {
    Node* k=root;
    if (k==nil)
        return k;
    while (k!=nil) {
        if (k->key==key) {
            return k;
        }
        else if (key<k->key)
            k=k->left;
        else
            k=k->right;
    }
    return nil;
}

void RbTree::leftRotate(Node *x) {
    //error checking
    if (x==nil || x->right==nil) {
        std::cout<<"An error has occured. Left rotation cannot be performed when the node x or it's right child are nill.\n";
        return;
    }
    //storing the right node (the one that moves in the place of x)
    Node* y=x->right;
    //the left subtree of y becomes the new right subtree of x
    x->right=y->left;
    if (y->left!=nil)
    y->left->parent=x;
    //the parent of x now becomes the parent of y
    y->parent=x->parent;
    if (x->parent==nil)
        root=y;
    else if (x==x->parent->left)
        x->parent->left=y;
    else
        x->parent->right=y;
    //finally, making the correct connection between x as the left child of y
    y->left=x;
    x->parent=y;
}

void RbTree::rightRotate(Node *x) {
//error checking
    if (x==nil || x->left==nil) {
        std::cout<<"An error has occured: cannot perform right rotation when x or it's left child is nill.\n";
        return;
    }
    //storing the left child of x, which will move up to replace it
    Node* y=x->left;
    //the right subtree of y moves to become the  left subtree of x
    x->left=y->right;
    if (y->right!=nil)
        y->right->parent=x;
//the parent of x becomes the parent of y, then we adjust so that the former parent of x also points to y
    y->parent=x->parent;
    if (x->parent==nil)
        root=y;
    else if (x==x->parent->left)
        x->parent->left=y;
    else
        x->parent->right=y;
    //moving x as the right ssubtree of y
    y->right=x;
    x->parent=y;
}

void RbTree::insert(int key) {
    Node* k=new Node;
    k->key=key;
    k->left=k->right=nil;
    k->color=RED;
    Node* p=nil;
    Node* x=root;
    while (x!=nil) {
        p=x;
        if (k->key<x->key)
            x=x->left;
        else
            x=x->right;
    }
    //now, p is the parent of the new node k
    if (p==nil)
        root=k;
    else if (k->key<p->key)
        p->left=k;
    else
        p->right=k;
    k->parent=p;
    insertFixup(k);
}

void RbTree::insertFixup(Node *z) {
    Node* p;
    Node* g;
    Node* u;

    while (z->parent->color == RED) {
        p = z->parent;
        g = p->parent;

        if (p == g->left) {
            u = g->right;

            if (u->color == RED) {
                p->color = BLACK;
                u->color = BLACK;
                g->color = RED;
                z = g;
            } else {
                if (z == p->right) {
                    z = p;
                    leftRotate(z);
                    p = z->parent;
                    g = p->parent;
                }
                p->color = BLACK;
                g->color = RED;
                rightRotate(g);
            }
        } else {
            u = g->left;

            if (u->color == RED) {
                p->color = BLACK;
                u->color = BLACK;
                g->color = RED;
                z = g;
            } else {
                if (z == p->left) {
                    z = p;
                    rightRotate(z);
                    p = z->parent;
                    g = p->parent;
                }
                p->color = BLACK;
                g->color = RED;
                leftRotate(g);
            }
        }
    }

    root->color = BLACK;
}

void RbTree::deleteNode(int key) {
    Node* z = search(key);
    if (z == nil)
        return;

    Node* y = z;
    Node* x;
    Color yOriginalColor = y->color;
    if (z->left == nil) {
        x = z->right;
        transplant(z, z->right);
    }
    else if (z->right == nil) {
        x = z->left;
        transplant(z, z->left);
    }
    else {
        y = minOnBranch(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        }
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (yOriginalColor == BLACK)
        deleteFixup(x);
    delete z;
}

void RbTree::transplant(Node *u, Node *v) {
    if (u->parent == nil)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    v->parent = u->parent;
}

void RbTree::deleteFixup(Node *x) {
    Node* w;

    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }

            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        }
        else {
            w = x->parent->left;

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }

            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }

    x->color = BLACK;
}

void RbTree::inorderPrint(Node *x) const {
    if (x == nil)
        return;

    inorderPrint(x->left);
    std::cout << x->key;
    if (x->color == RED)
        std::cout << "(R) ";
    else
        std::cout << "(B) ";
    inorderPrint(x->right);
}

void RbTree::showTree() const {
    if (root == nil) {
        std::cout << "Tree is empty.\n";
        return;
    }
    std::cout << "Tree in inorder: ";
    inorderPrint(root);
    std::cout << "\n";
}
