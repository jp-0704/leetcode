// AVL Tree — self-balancing BST, O(log n) insert/delete/search
#include <iostream>
#include <vector>
#include <algorithm>

struct AVLNode {
    int val, height;
    AVLNode* left;
    AVLNode* right;
    AVLNode(int v) : val(v), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree {
private:
    AVLNode* root;

    int height(AVLNode* n) { return n ? n->height : 0; }
    int balanceFactor(AVLNode* n) { return n ? height(n->left) - height(n->right) : 0; }

    void updateHeight(AVLNode* n) {
        n->height = 1 + std::max(height(n->left), height(n->right));
    }

    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        x->right = y; y->left = T2;
        updateHeight(y); updateHeight(x);
        return x;
    }

    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        y->left = x; x->right = T2;
        updateHeight(x); updateHeight(y);
        return y;
    }

    AVLNode* balance(AVLNode* node) {
        updateHeight(node);
        int bf = balanceFactor(node);
        // Left heavy
        if (bf > 1) {
            if (balanceFactor(node->left) < 0)
                node->left = rotateLeft(node->left); // LR
            return rotateRight(node);
        }
        // Right heavy
        if (bf < -1) {
            if (balanceFactor(node->right) > 0)
                node->right = rotateRight(node->right); // RL
            return rotateLeft(node);
        }
        return node;
    }

    AVLNode* insert(AVLNode* node, int val) {
        if (!node) return new AVLNode(val);
        if (val < node->val) node->left  = insert(node->left, val);
        else if (val > node->val) node->right = insert(node->right, val);
        else return node; // duplicate
        return balance(node);
    }

    AVLNode* minNode(AVLNode* node) {
        while (node->left) node = node->left;
        return node;
    }

    AVLNode* deleteNode(AVLNode* node, int val) {
        if (!node) return nullptr;
        if (val < node->val) node->left  = deleteNode(node->left, val);
        else if (val > node->val) node->right = deleteNode(node->right, val);
        else {
            if (!node->left || !node->right) {
                AVLNode* tmp = node->left ? node->left : node->right;
                delete node;
                return tmp;
            }
            AVLNode* succ = minNode(node->right);
            node->val = succ->val;
            node->right = deleteNode(node->right, succ->val);
        }
        return balance(node);
    }

    bool search(AVLNode* node, int val) const {
        if (!node) return false;
        if (val == node->val) return true;
        return val < node->val ? search(node->left, val) : search(node->right, val);
    }

    void inorder(AVLNode* node, std::vector<int>& res) const {
        if (!node) return;
        inorder(node->left, res);
        res.push_back(node->val);
        inorder(node->right, res);
    }

    void destroy(AVLNode* node) {
        if (!node) return;
        destroy(node->left); destroy(node->right); delete node;
    }

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { destroy(root); }

    void insert(int val) { root = insert(root, val); }
    void remove(int val) { root = deleteNode(root, val); }
    bool search(int val) const { return search(root, val); }
    int height() const { return height(root); }

    std::vector<int> inorder() const {
        std::vector<int> res; inorder(root, res); return res;
    }
};

void print(const std::vector<int>& v) {
    for (int x : v) std::cout << x << " "; std::cout << "\n";
}

int main() {
    AVLTree avl;
    for (int x : {10, 20, 30, 40, 50, 25}) avl.insert(x);
    std::cout << "Inorder: "; print(avl.inorder());
    std::cout << "Height: " << avl.height() << "\n"; // Should be 3 (balanced)
    std::cout << "Search 25: " << avl.search(25) << "\n"; // 1
    avl.remove(30);
    std::cout << "After delete 30: "; print(avl.inorder());
    return 0;
}
