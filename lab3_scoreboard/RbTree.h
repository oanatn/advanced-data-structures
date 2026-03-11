#include <string>

enum Color {RED, BLACK};

struct Node {
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int key;
    std::string name;
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
void topK(Node* x, int &k) const;

public:
    RbTree();
    Node *search(int key,std::string name) const;
    void deleteNode(int key,std::string name);
    Node* minimum() const;
    Node* maximum() const;
    void insert(int key, std::string name);
    void top(int k) const;
    void showTree() const;
};

