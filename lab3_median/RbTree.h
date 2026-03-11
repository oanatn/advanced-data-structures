enum Color {RED, BLACK};

struct Node {
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int key;
    int size;
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
    Node* kthNode(Node* x, int k) const;
    void inorderPrint(Node* x) const;
    void fixSizeUpwards(Node *x); //for adjusting sizes after removing a node

public:
    RbTree();
    Node* search(int key) const;
    Node* minimum() const;
    Node* maximum() const;
    void insert(int key);
    void deleteNode(int key);
    int kth(int k) const;
    int median() const;
    void showTree() const;
};

