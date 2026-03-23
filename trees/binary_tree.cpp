// Binary Tree — all traversals, height, diameter, LCA, serialization
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <string>
#include <sstream>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// ── Traversals ────────────────────────────────────────────────────────────────
void inorder(TreeNode* root, std::vector<int>& res) {
    if (!root) return;
    inorder(root->left, res);
    res.push_back(root->val);
    inorder(root->right, res);
}

void preorder(TreeNode* root, std::vector<int>& res) {
    if (!root) return;
    res.push_back(root->val);
    preorder(root->left, res);
    preorder(root->right, res);
}

void postorder(TreeNode* root, std::vector<int>& res) {
    if (!root) return;
    postorder(root->left, res);
    postorder(root->right, res);
    res.push_back(root->val);
}

// Iterative inorder (Morris traversal — O(1) space)
std::vector<int> morrisInorder(TreeNode* root) {
    std::vector<int> res;
    TreeNode* cur = root;
    while (cur) {
        if (!cur->left) { res.push_back(cur->val); cur = cur->right; }
        else {
            TreeNode* pred = cur->left;
            while (pred->right && pred->right != cur) pred = pred->right;
            if (!pred->right) { pred->right = cur; cur = cur->left; }
            else { pred->right = nullptr; res.push_back(cur->val); cur = cur->right; }
        }
    }
    return res;
}

// Level-order (BFS)
std::vector<std::vector<int>> levelOrder(TreeNode* root) {
    std::vector<std::vector<int>> res;
    if (!root) return res;
    std::queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int sz = q.size();
        std::vector<int> level;
        for (int i = 0; i < sz; ++i) {
            TreeNode* node = q.front(); q.pop();
            level.push_back(node->val);
            if (node->left)  q.push(node->left);
            if (node->right) q.push(node->right);
        }
        res.push_back(level);
    }
    return res;
}

// Zigzag level order
std::vector<std::vector<int>> zigzagLevelOrder(TreeNode* root) {
    std::vector<std::vector<int>> res;
    if (!root) return res;
    std::queue<TreeNode*> q;
    q.push(root);
    bool leftToRight = true;
    while (!q.empty()) {
        int sz = q.size();
        std::vector<int> level(sz);
        for (int i = 0; i < sz; ++i) {
            TreeNode* node = q.front(); q.pop();
            int idx = leftToRight ? i : sz - 1 - i;
            level[idx] = node->val;
            if (node->left)  q.push(node->left);
            if (node->right) q.push(node->right);
        }
        res.push_back(level);
        leftToRight = !leftToRight;
    }
    return res;
}

// ── Properties ────────────────────────────────────────────────────────────────
int height(TreeNode* root) {
    if (!root) return 0;
    return 1 + std::max(height(root->left), height(root->right));
}

int diameter(TreeNode* root, int& maxDiam) {
    if (!root) return 0;
    int l = diameter(root->left, maxDiam);
    int r = diameter(root->right, maxDiam);
    maxDiam = std::max(maxDiam, l + r);
    return 1 + std::max(l, r);
}

int getDiameter(TreeNode* root) {
    int d = 0;
    diameter(root, d);
    return d;
}

bool isBalanced(TreeNode* root, int& ht) {
    if (!root) { ht = 0; return true; }
    int lh, rh;
    if (!isBalanced(root->left, lh) || !isBalanced(root->right, rh)) return false;
    ht = 1 + std::max(lh, rh);
    return std::abs(lh - rh) <= 1;
}

// Lowest Common Ancestor
TreeNode* lca(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;
    TreeNode* left  = lca(root->left, p, q);
    TreeNode* right = lca(root->right, p, q);
    if (left && right) return root;
    return left ? left : right;
}

// Max path sum
int maxPathSum(TreeNode* root, int& res) {
    if (!root) return 0;
    int l = std::max(0, maxPathSum(root->left, res));
    int r = std::max(0, maxPathSum(root->right, res));
    res = std::max(res, l + r + root->val);
    return root->val + std::max(l, r);
}

// ── Serialization / Deserialization ───────────────────────────────────────────
std::string serialize(TreeNode* root) {
    if (!root) return "null";
    return std::to_string(root->val) + "," +
           serialize(root->left) + "," + serialize(root->right);
}

TreeNode* deserialize(std::istringstream& ss) {
    std::string token;
    if (!std::getline(ss, token, ',')) return nullptr;
    if (token == "null") return nullptr;
    TreeNode* node = new TreeNode(std::stoi(token));
    node->left  = deserialize(ss);
    node->right = deserialize(ss);
    return node;
}

TreeNode* deserialize(const std::string& data) {
    std::istringstream ss(data);
    return deserialize(ss);
}

// ── Helpers ───────────────────────────────────────────────────────────────────
void print(const std::vector<int>& v) {
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5
    TreeNode* root = new TreeNode(1);
    root->left  = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left  = new TreeNode(4);
    root->left->right = new TreeNode(5);

    std::vector<int> in, pre, post;
    inorder(root, in);   std::cout << "Inorder:   "; print(in);
    preorder(root, pre); std::cout << "Preorder:  "; print(pre);
    postorder(root, post);std::cout << "Postorder: "; print(post);

    auto levels = levelOrder(root);
    std::cout << "Level order: ";
    for (auto& lv : levels) { for (int x : lv) std::cout << x << " "; std::cout << "| "; }
    std::cout << "\n";

    std::cout << "Height: " << height(root) << "\n";        // 3
    std::cout << "Diameter: " << getDiameter(root) << "\n"; // 3

    auto serial = serialize(root);
    std::cout << "Serialized: " << serial << "\n";
    TreeNode* restored = deserialize(serial);
    std::vector<int> r;
    inorder(restored, r);
    std::cout << "Restored inorder: "; print(r);
    return 0;
}
