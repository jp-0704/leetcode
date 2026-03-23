// Red-Black Tree — self-balancing BST with O(log n) operations
// Properties: 1) Every node is Red or Black
//             2) Root is Black
//             3) Red nodes have Black children
//             4) All paths from node to NULL have equal Black depth
#include <iostream>
#include <vector>

enum Color { RED, BLACK };

struct RBNode {
    int val;
    Color color;
    RBNode *left, *right, *parent;
    RBNode(int v) : val(v), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    RBNode* root;
    RBNode* NIL; // sentinel node

    void rotateLeft(RBNode* x) {
        RBNode* y = x->right;
        x->right = y->left;
        if (y->left != NIL) y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x; x->parent = y;
    }

    void rotateRight(RBNode* x) {
        RBNode* y = x->left;
        x->left = y->right;
        if (y->right != NIL) y->right->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->right) x->parent->right = y;
        else x->parent->left = y;
        y->right = x; x->parent = y;
    }

    void fixInsert(RBNode* z) {
        while (z->parent && z->parent->color == RED) {
            RBNode* gp = z->parent->parent;
            if (z->parent == gp->left) {
                RBNode* uncle = gp->right;
                if (uncle->color == RED) { // Case 1: uncle is red
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    gp->color = RED;
                    z = gp;
                } else {
                    if (z == z->parent->right) { z = z->parent; rotateLeft(z); } // Case 2
                    z->parent->color = BLACK; gp->color = RED; rotateRight(gp);   // Case 3
                }
            } else {
                RBNode* uncle = gp->left;
                if (uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    gp->color = RED;
                    z = gp;
                } else {
                    if (z == z->parent->left) { z = z->parent; rotateRight(z); }
                    z->parent->color = BLACK; gp->color = RED; rotateLeft(gp);
                }
            }
        }
        root->color = BLACK;
    }

    void transplant(RBNode* u, RBNode* v) {
        if (!u->parent) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }

    RBNode* minimum(RBNode* node) {
        while (node->left != NIL) node = node->left;
        return node;
    }

    void fixDelete(RBNode* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                RBNode* w = x->parent->right;
                if (w->color == RED) { w->color = BLACK; x->parent->color = RED; rotateLeft(x->parent); w = x->parent->right; }
                if (w->left->color == BLACK && w->right->color == BLACK) { w->color = RED; x = x->parent; }
                else {
                    if (w->right->color == BLACK) { w->left->color = BLACK; w->color = RED; rotateRight(w); w = x->parent->right; }
                    w->color = x->parent->color; x->parent->color = BLACK; w->right->color = BLACK; rotateLeft(x->parent); x = root;
                }
            } else {
                RBNode* w = x->parent->left;
                if (w->color == RED) { w->color = BLACK; x->parent->color = RED; rotateRight(x->parent); w = x->parent->left; }
                if (w->right->color == BLACK && w->left->color == BLACK) { w->color = RED; x = x->parent; }
                else {
                    if (w->left->color == BLACK) { w->right->color = BLACK; w->color = RED; rotateLeft(w); w = x->parent->left; }
                    w->color = x->parent->color; x->parent->color = BLACK; w->left->color = BLACK; rotateRight(x->parent); x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void deleteNode(RBNode* z) {
        RBNode* y = z, *x;
        Color orig = y->color;
        if (z->left == NIL) { x = z->right; transplant(z, z->right); }
        else if (z->right == NIL) { x = z->left; transplant(z, z->left); }
        else {
            y = minimum(z->right); orig = y->color; x = y->right;
            if (y->parent == z) x->parent = y;
            else { transplant(y, y->right); y->right = z->right; y->right->parent = y; }
            transplant(z, y); y->left = z->left; y->left->parent = y; y->color = z->color;
        }
        delete z;
        if (orig == BLACK) fixDelete(x);
    }

    void inorder(RBNode* node, std::vector<int>& res) const {
        if (node == NIL) return;
        inorder(node->left, res);
        res.push_back(node->val);
        inorder(node->right, res);
    }

    void destroy(RBNode* node) {
        if (node == NIL) return;
        destroy(node->left); destroy(node->right); delete node;
    }

public:
    RedBlackTree() {
        NIL = new RBNode(0); NIL->color = BLACK;
        root = NIL;
    }

    ~RedBlackTree() { destroy(root); delete NIL; }

    void insert(int val) {
        RBNode* z = new RBNode(val);
        z->left = z->right = NIL;
        RBNode* y = nullptr, *x = root;
        while (x != NIL) { y = x; x = (z->val < x->val) ? x->left : x->right; }
        z->parent = y;
        if (!y) root = z;
        else if (z->val < y->val) y->left = z;
        else y->right = z;
        fixInsert(z);
    }

    bool search(int val) const {
        RBNode* cur = root;
        while (cur != NIL) {
            if (val == cur->val) return true;
            cur = (val < cur->val) ? cur->left : cur->right;
        }
        return false;
    }

    void remove(int val) {
        RBNode* z = root;
        while (z != NIL && z->val != val)
            z = (val < z->val) ? z->left : z->right;
        if (z != NIL) deleteNode(z);
    }

    std::vector<int> inorder() const {
        std::vector<int> res; inorder(root, res); return res;
    }
};

int main() {
    RedBlackTree rbt;
    for (int x : {7, 3, 18, 10, 22, 8, 11, 26}) rbt.insert(x);
    auto v = rbt.inorder();
    std::cout << "Inorder: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
    std::cout << "Search 10: " << rbt.search(10) << "\n"; // 1
    rbt.remove(10);
    v = rbt.inorder();
    std::cout << "After delete 10: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
    return 0;
}
