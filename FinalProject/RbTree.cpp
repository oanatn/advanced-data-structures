#include"RbTree.h"

#include <iostream>

bool lessThan(int key1,std::string name1,int key2,std::string name2) {
    if (key1!=key2)
        return key1<key2;
    return name1<name2;
}

bool equalTo(int key1,std::string name1,int key2,std::string name2) {
    return key1==key2 && name1==name2;
}

RbTree::RbTree() {
    this->logger = nullptr;
    this->nil=new Node;
    this->nil->color=BLACK;
    this->nil->left=this->nil->right=nil;
    this->nil->parent=nil;
    this->nil->key=-1;
    this->nil->name="";
    this->root=nil;
}

void RbTree::setLogger(OperationLogger* logger) {
    this->logger = logger;
}

void RbTree::topK(Node *x, int &k) const {
    if (x==nil || k==0)
        return;
    topK(x->right, k);
    if (k>0) {
        std::cout<<x->name<<" "<<x->key<<"\n";
        k--;
    }
    topK(x->left, k);
}

void RbTree::top(int k) const {
    topK(root,k);
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

Node *RbTree::search(int key,std::string name) const {
    Node* k=root;
    if (k==nil)
        return k;
    while (k!=nil) {
        if (equalTo(k->key,k->name,key,name)) {
            return k;
        }
        else if (lessThan(key,name,k->key,k->name))
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

    if (logger) {
        logger->log("Red-black tree: performing left rotation around node " + x->name + ".");
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
        std::cout<<"An error has occured: cannot perform right rotation when x or it's left child are nill.\n";
        return;
    }

    if (logger) {
        logger->log("Red-black tree: performing right rotation around node " + x->name + ".");
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

void RbTree::insert(int key, std::string name) {
    if (logger) {
        logger->log("Red-black tree: inserting " + name + " with score " + std::to_string(key) + ".");
    }

    Node* k=new Node;
    k->key=key;
    k->left=k->right=nil;
    k->name=name;
    k->color=RED;

    if (logger) {
        logger->log("Red-black tree: new nodes are inserted as RED.");
    }

    Node* p=nil;
    Node* x=root;
    while (x!=nil) {
        p=x;
        if (lessThan(k->key,k->name,x->key,x->name)) {
            if (logger) {
                logger->log("Red-black tree: " + name + " goes left of " + x->name + ".");
            }
            x=x->left;
        }
        else {
            if (logger) {
                logger->log("Red-black tree: " + name + " goes right of " + x->name + ".");
            }
            x=x->right;
        }
    }
    //now, p is the parent of the new node k
    if (p==nil) {
        root=k;
        if (logger) {
            logger->log("Red-black tree: inserted node becomes the root.");
        }
    }
    else if (lessThan(k->key,k->name,p->key,p->name)) {
        p->left=k;
        if (logger) {
            logger->log("Red-black tree: inserted as left child of " + p->name + ".");
        }
    }
    else {
        p->right=k;
        if (logger) {
            logger->log("Red-black tree: inserted as right child of " + p->name + ".");
        }
    }
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

        if (logger) {
            logger->log("Red-black tree: violation found because parent " + p->name + " is RED.");
        }

        if (p == g->left) {
            u = g->right;

            if (logger) {
                logger->log("Red-black tree: parent is the left child of grandparent " + g->name + ".");
            }

            if (u->color == RED) {
                if (logger) {
                    logger->log("Red-black tree: uncle is RED, so parent and uncle become BLACK, grandparent becomes RED.");
                }

                p->color = BLACK;
                u->color = BLACK;
                g->color = RED;
                z = g;
            } else {
                if (logger) {
                    logger->log("Red-black tree: uncle is BLACK, so rotations may be needed.");
                }

                if (z == p->right) {
                    if (logger) {
                        logger->log("Red-black tree: left-right case detected.");
                    }

                    z = p;
                    leftRotate(z);
                    p = z->parent;
                    g = p->parent;
                }

                if (logger) {
                    logger->log("Red-black tree: recoloring parent BLACK and grandparent RED.");
                }

                p->color = BLACK;
                g->color = RED;
                rightRotate(g);
            }
        } else {
            u = g->left;

            if (logger) {
                logger->log("Red-black tree: parent is the right child of grandparent " + g->name + ".");
            }

            if (u->color == RED) {
                if (logger) {
                    logger->log("Red-black tree: uncle is RED, so parent and uncle become BLACK, grandparent becomes RED.");
                }

                p->color = BLACK;
                u->color = BLACK;
                g->color = RED;
                z = g;
            } else {
                if (logger) {
                    logger->log("Red-black tree: uncle is BLACK, so rotations may be needed.");
                }

                if (z == p->left) {
                    if (logger) {
                        logger->log("Red-black tree: right-left case detected.");
                    }

                    z = p;
                    rightRotate(z);
                    p = z->parent;
                    g = p->parent;
                }

                if (logger) {
                    logger->log("Red-black tree: recoloring parent BLACK and grandparent RED.");
                }

                p->color = BLACK;
                g->color = RED;
                leftRotate(g);
            }
        }
    }

    root->color = BLACK;

    if (logger) {
        logger->log("Red-black tree: root is set to BLACK.");
        logger->log("Red-black tree: insertion fix-up complete.");
    }
}

void RbTree::deleteNode(int key,std::string name) {
    if (logger) {
        logger->log("Red-black tree: deleting " + name + " with score " + std::to_string(key) + ".");
    }

    Node* z = search(key,name);
    if (z == nil) {
        if (logger) {
            logger->log("Red-black tree: node was not found, no deletion performed.");
        }
        return;
    }

    Node* y = z;
    Node* x;
    Color yOriginalColor = y->color;
    if (z->left == nil) {
        if (logger) {
            logger->log("Red-black tree: node has no left child, replacing it with its right subtree.");
        }

        x = z->right;
        transplant(z, z->right);
    }
    else if (z->right == nil) {
        if (logger) {
            logger->log("Red-black tree: node has no right child, replacing it with its left subtree.");
        }

        x = z->left;
        transplant(z, z->left);
    }
    else {
        if (logger) {
            logger->log("Red-black tree: node has two children, using its successor.");
        }

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
    if (yOriginalColor == BLACK) {
        if (logger) {
            logger->log("Red-black tree: deleted node was BLACK, delete fix-up is needed.");
        }

        deleteFixup(x);
    }
    else {
        if (logger) {
            logger->log("Red-black tree: deleted node was RED, no delete fix-up needed.");
        }
    }

    delete z;

    if (logger) {
        logger->log("Red-black tree: deletion complete.");
    }
}

void RbTree::transplant(Node *u, Node *v) {
    if (logger) {
        logger->log("Red-black tree: transplanting subtree during deletion.");
    }

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
        if (logger) {
            logger->log("Red-black tree: fixing double-black situation after deletion.");
        }

        if (x == x->parent->left) {
            w = x->parent->right;

            if (logger) {
                logger->log("Red-black tree: current node is a left child; sibling is on the right.");
            }

            if (w->color == RED) {
                if (logger) {
                    logger->log("Red-black tree: sibling is RED, recoloring and rotating left.");
                }

                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }

            if (w->left->color == BLACK && w->right->color == BLACK) {
                if (logger) {
                    logger->log("Red-black tree: sibling and its children are BLACK, pushing problem upward.");
                }

                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->right->color == BLACK) {
                    if (logger) {
                        logger->log("Red-black tree: sibling's far child is BLACK, rotating sibling right.");
                    }

                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = x->parent->right;
                }

                if (logger) {
                    logger->log("Red-black tree: final deletion fix-up recoloring and left rotation.");
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

            if (logger) {
                logger->log("Red-black tree: current node is a right child; sibling is on the left.");
            }

            if (w->color == RED) {
                if (logger) {
                    logger->log("Red-black tree: sibling is RED, recoloring and rotating right.");
                }

                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }

            if (w->right->color == BLACK && w->left->color == BLACK) {
                if (logger) {
                    logger->log("Red-black tree: sibling and its children are BLACK, pushing problem upward.");
                }

                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->left->color == BLACK) {
                    if (logger) {
                        logger->log("Red-black tree: sibling's far child is BLACK, rotating sibling left.");
                    }

                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = x->parent->left;
                }

                if (logger) {
                    logger->log("Red-black tree: final deletion fix-up recoloring and right rotation.");
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

    if (logger) {
        logger->log("Red-black tree: delete fix-up complete.");
    }
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

void RbTree::printStructure(Node *x, int depth) const {
    if (x == nil)
        return;

    printStructure(x->right, depth + 1);

    for (int i = 0; i < depth; i++) {
        std::cout << "    ";
    }

    std::cout << x->name << "(" << x->key;

    if (x->color == RED)
        std::cout << ",R";
    else
        std::cout << ",B";

    std::cout << ")\n";

    printStructure(x->left, depth + 1);
}

void RbTree::showTree() const {
    if (root == nil) {
        std::cout << "Tree is empty.\n";
        return;
    }

    std::cout << "\nRed-Black Tree Structure:\n\n";

    printStructure(root, 0);

    std::cout << "\n";
}