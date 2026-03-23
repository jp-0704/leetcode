// B-Tree of order t (each node has [t-1, 2t-1] keys except root)
// O(log n) search, insert, delete — used in databases and file systems
#include <iostream>
#include <vector>
#include <algorithm>

class BTree {
private:
    struct BNode {
        std::vector<int> keys;
        std::vector<BNode*> children;
        bool leaf;

        BNode(bool isLeaf) : leaf(isLeaf) {}
    };

    BNode* root;
    int t; // minimum degree

    BNode* search(BNode* x, int k) const {
        int i = 0;
        while (i < (int)x->keys.size() && k > x->keys[i]) ++i;
        if (i < (int)x->keys.size() && k == x->keys[i]) return x;
        if (x->leaf) return nullptr;
        return search(x->children[i], k);
    }

    void splitChild(BNode* x, int i) {
        BNode* y = x->children[i];
        BNode* z = new BNode(y->leaf);
        x->keys.insert(x->keys.begin() + i, y->keys[t - 1]);
        x->children.insert(x->children.begin() + i + 1, z);
        z->keys.assign(y->keys.begin() + t, y->keys.end());
        y->keys.resize(t - 1);
        if (!y->leaf) {
            z->children.assign(y->children.begin() + t, y->children.end());
            y->children.resize(t);
        }
    }

    void insertNonFull(BNode* x, int k) {
        int i = x->keys.size() - 1;
        if (x->leaf) {
            x->keys.push_back(0);
            while (i >= 0 && k < x->keys[i]) { x->keys[i+1] = x->keys[i]; --i; }
            x->keys[i + 1] = k;
        } else {
            while (i >= 0 && k < x->keys[i]) --i;
            ++i;
            if ((int)x->children[i]->keys.size() == 2*t - 1) {
                splitChild(x, i);
                if (k > x->keys[i]) ++i;
            }
            insertNonFull(x->children[i], k);
        }
    }

    void traverse(BNode* x, std::vector<int>& res) const {
        int i;
        for (i = 0; i < (int)x->keys.size(); ++i) {
            if (!x->leaf) traverse(x->children[i], res);
            res.push_back(x->keys[i]);
        }
        if (!x->leaf) traverse(x->children[i], res);
    }

    void destroy(BNode* x) {
        if (!x->leaf) for (auto* c : x->children) destroy(c);
        delete x;
    }

public:
    BTree(int degree) : t(degree) {
        root = new BNode(true);
    }

    ~BTree() { destroy(root); }

    bool search(int k) const { return search(root, k) != nullptr; }

    void insert(int k) {
        BNode* r = root;
        if ((int)r->keys.size() == 2*t - 1) {
            BNode* s = new BNode(false);
            root = s;
            s->children.push_back(r);
            splitChild(s, 0);
            insertNonFull(s, k);
        } else {
            insertNonFull(r, k);
        }
    }

    std::vector<int> traverse() const {
        std::vector<int> res;
        traverse(root, res);
        return res;
    }
};

int main() {
    BTree bt(3); // minimum degree 3 (max 5 keys per node)
    for (int x : {10, 20, 5, 6, 12, 30, 7, 17}) bt.insert(x);

    auto keys = bt.traverse();
    std::cout << "B-Tree inorder: ";
    for (int x : keys) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "Search 6: "  << bt.search(6)  << "\n"; // 1
    std::cout << "Search 15: " << bt.search(15) << "\n"; // 0
    return 0;
}
