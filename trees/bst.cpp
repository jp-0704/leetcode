// Binary Search Tree — insert, search, delete, traversals, validation
#include <iostream>
#include <vector>
#include <climits>

struct Node {
    int val;
    Node* left;
    Node* right;
    Node(int v) : val(v), left(nullptr), right(nullptr) {}
};

class BST {
private:
    Node* root;

    Node* insert(Node* node, int val) {
        if (!node) return new Node(val);
        if (val < node->val) node->left  = insert(node->left, val);
        else if (val > node->val) node->right = insert(node->right, val);
        return node;
    }

    Node* minNode(Node* node) {
        while (node->left) node = node->left;
        return node;
    }

    Node* deleteNode(Node* node, int val) {
        if (!node) return nullptr;
        if (val < node->val) node->left  = deleteNode(node->left, val);
        else if (val > node->val) node->right = deleteNode(node->right, val);
        else {
            if (!node->left)  { Node* t = node->right; delete node; return t; }
            if (!node->right) { Node* t = node->left;  delete node; return t; }
            Node* succ = minNode(node->right);
            node->val   = succ->val;
            node->right = deleteNode(node->right, succ->val);
        }
        return node;
    }

    bool search(Node* node, int val) const {
        if (!node) return false;
        if (val == node->val) return true;
        if (val < node->val) return search(node->left, val);
        return search(node->right, val);
    }

    bool isValid(Node* node, long long lo, long long hi) const {
        if (!node) return true;
        if (node->val <= lo || node->val >= hi) return false;
        return isValid(node->left, lo, node->val) &&
               isValid(node->right, node->val, hi);
    }

    void inorder(Node* node, std::vector<int>& res) const {
        if (!node) return;
        inorder(node->left, res);
        res.push_back(node->val);
        inorder(node->right, res);
    }

    Node* lca(Node* node, int p, int q) const {
        if (!node) return nullptr;
        if (p < node->val && q < node->val) return lca(node->left, p, q);
        if (p > node->val && q > node->val) return lca(node->right, p, q);
        return node;
    }

    int kthSmallest(Node* node, int& k) const {
        if (!node) return -1;
        int left = kthSmallest(node->left, k);
        if (k == 0) return left;
        if (--k == 0) return node->val;
        return kthSmallest(node->right, k);
    }

    // Floor: largest value <= key
    Node* floor(Node* node, int key) const {
        if (!node) return nullptr;
        if (node->val == key) return node;
        if (node->val > key) return floor(node->left, key);
        Node* right = floor(node->right, key);
        return right ? right : node;
    }

    // Ceil: smallest value >= key
    Node* ceil(Node* node, int key) const {
        if (!node) return nullptr;
        if (node->val == key) return node;
        if (node->val < key) return ceil(node->right, key);
        Node* left = ceil(node->left, key);
        return left ? left : node;
    }

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}
    ~BST() { destroy(root); }

    void insert(int val) { root = insert(root, val); }
    void remove(int val) { root = deleteNode(root, val); }
    bool search(int val) const { return search(root, val); }
    bool isValid() const { return isValid(root, LLONG_MIN, LLONG_MAX); }

    std::vector<int> inorder() const {
        std::vector<int> res;
        inorder(root, res);
        return res;
    }

    int lcaVal(int p, int q) const {
        Node* n = lca(root, p, q);
        return n ? n->val : -1;
    }

    int kthSmallest(int k) const { return kthSmallest(root, k); }

    int floorVal(int key) const {
        Node* n = floor(root, key);
        return n ? n->val : INT_MIN;
    }

    int ceilVal(int key) const {
        Node* n = ceil(root, key);
        return n ? n->val : INT_MAX;
    }
};

void print(const std::vector<int>& v) {
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    BST bst;
    for (int x : {5, 3, 7, 1, 4, 6, 8}) bst.insert(x);

    std::cout << "Inorder (sorted): "; print(bst.inorder());
    std::cout << "Search 4: " << bst.search(4) << "\n";    // 1
    std::cout << "Is valid BST: " << bst.isValid() << "\n"; // 1
    std::cout << "LCA(1,4): " << bst.lcaVal(1, 4) << "\n"; // 3
    std::cout << "3rd smallest: " << bst.kthSmallest(3) << "\n"; // 4
    std::cout << "Floor(5): " << bst.floorVal(5) << "\n";  // 5
    std::cout << "Ceil(5):  " << bst.ceilVal(5)  << "\n";  // 5
    std::cout << "Floor(4.5→4): " << bst.floorVal(4) << "\n"; // 4

    bst.remove(3);
    std::cout << "After delete 3: "; print(bst.inorder());
    return 0;
}
