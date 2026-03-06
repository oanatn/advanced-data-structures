enum Color {RED, BLACK};

struct Node {
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int key;
    Color color;
};

class RbTree {
private:
    Node* root;
    Node* nil;

    void leftRotate(Node* x);
    void rightRotate(Node* x);
    void insertFixup(Node* z);
    void deleteFixup(Node* x);
    void transplant(Node* u, Node* v);
    Node* minOnBranch(Node* x) const;
    Node* maxOnBranch(Node* x) const;
    void inorderPrint(Node* x) const;

public:
    RbTree();
    Node* search(int key) const;
    Node* minimum() const;
    Node* maximum() const;
    void insert(int key);
    void deleteNode(int key);
    void showTree() const;
};

