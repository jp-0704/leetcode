// Sparse Table — range minimum/maximum queries in O(1) after O(n log n) build
// Also: LCA with Euler tour + sparse table
#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <functional>

// ── Sparse Table for Range Minimum Query (RMQ) ────────────────────────────────
class SparseTable {
private:
    std::vector<std::vector<int>> table;
    std::vector<int> log2_floor;
    std::function<int(int,int)> fn;

public:
    SparseTable(const std::vector<int>& arr, std::function<int(int,int)> combine)
        : fn(combine) {
        int n = arr.size();
        int LOG = std::log2(n) + 1;
        table.assign(LOG, std::vector<int>(n));
        log2_floor.assign(n+1, 0);
        for (int i = 2; i <= n; ++i) log2_floor[i] = log2_floor[i/2] + 1;

        table[0] = arr;
        for (int j = 1; j < LOG; ++j)
            for (int i = 0; i + (1<<j) <= n; ++i)
                table[j][i] = fn(table[j-1][i], table[j-1][i + (1<<(j-1))]);
    }

    // Query [l, r] — O(1) for idempotent functions (min, max, gcd)
    int query(int l, int r) const {
        int k = log2_floor[r - l + 1];
        return fn(table[k][l], table[k][r - (1<<k) + 1]);
    }
};

// ── LCA using Euler Tour + Sparse Table ──────────────────────────────────────
struct TreeNode {
    int val;
    std::vector<int> children;
    TreeNode(int v) : val(v) {}
};

class LCATree {
    int n;
    std::vector<std::vector<int>> adj;
    std::vector<int> euler, depth, first;
    std::unique_ptr<SparseTable> rmq;

    void dfs(int u, int par, int d) {
        first[u] = euler.size();
        euler.push_back(u);
        depth[u] = d;
        for (int v : adj[u]) {
            if (v != par) {
                dfs(v, u, d+1);
                euler.push_back(u);
            }
        }
    }

public:
    LCATree(int n, const std::vector<std::pair<int,int>>& edges)
        : n(n), adj(n), depth(n), first(n) {
        for (auto [u,v] : edges) { adj[u].push_back(v); adj[v].push_back(u); }
        euler.reserve(2*n);
        dfs(0, -1, 0);
        std::vector<int> depthInEuler;
        for (int u : euler) depthInEuler.push_back(depth[u]);
        rmq = std::make_unique<SparseTable>(depthInEuler, [](int a, int b){ return std::min(a,b); });
    }

    int lca(int u, int v) const {
        int l = first[u], r = first[v];
        if (l > r) std::swap(l, r);
        int minDepth = rmq->query(l, r);
        // Find node with that depth in euler[l..r]
        for (int i = l; i <= r; ++i)
            if (depth[euler[i]] == minDepth) return euler[i];
        return -1;
    }

    int dist(int u, int v) const {
        return depth[u] + depth[v] - 2*depth[lca(u,v)];
    }
};

// ── Binary Lifting LCA (O(n log n) build, O(log n) query) ────────────────────
class BinaryLiftLCA {
    int n, LOG;
    std::vector<std::vector<int>> up; // up[v][j] = 2^j-th ancestor
    std::vector<int> depth;
    std::vector<std::vector<int>> adj;

    void dfs(int u, int par) {
        up[u][0] = par;
        for (int j = 1; j < LOG; ++j)
            up[u][j] = up[up[u][j-1]][j-1];
        for (int v : adj[u])
            if (v != par) { depth[v] = depth[u]+1; dfs(v, u); }
    }

public:
    BinaryLiftLCA(int n, const std::vector<std::pair<int,int>>& edges)
        : n(n), LOG(std::log2(n)+2), up(n, std::vector<int>(LOG, 0)), depth(n, 0), adj(n) {
        for (auto [u,v] : edges) { adj[u].push_back(v); adj[v].push_back(u); }
        dfs(0, 0);
    }

    int lca(int u, int v) const {
        if (depth[u] < depth[v]) std::swap(u, v);
        int diff = depth[u] - depth[v];
        for (int j = 0; j < LOG; ++j) if ((diff>>j)&1) u = up[u][j];
        if (u == v) return u;
        for (int j = LOG-1; j >= 0; --j)
            if (up[u][j] != up[v][j]) { u = up[u][j]; v = up[v][j]; }
        return up[u][0];
    }

    int dist(int u, int v) const { return depth[u] + depth[v] - 2*depth[lca(u,v)]; }
};

int main() {
    // RMQ
    std::vector<int> arr = {2, 4, 3, 1, 6, 7, 8, 9, 1, 7};
    SparseTable rmq(arr, [](int a, int b){ return std::min(a,b); });
    std::cout << "Min [2,8]: " << rmq.query(2, 8) << "\n"; // 1
    std::cout << "Min [0,4]: " << rmq.query(0, 4) << "\n"; // 1

    SparseTable rmqMax(arr, [](int a, int b){ return std::max(a,b); });
    std::cout << "Max [0,7]: " << rmqMax.query(0, 7) << "\n"; // 9

    // LCA
    std::vector<std::pair<int,int>> edges = {{0,1},{0,2},{1,3},{1,4},{2,5},{2,6}};
    BinaryLiftLCA lca(7, edges);
    std::cout << "\nLCA(3,4): " << lca.lca(3,4) << "\n"; // 1
    std::cout << "LCA(3,5): " << lca.lca(3,5) << "\n"; // 0
    std::cout << "Dist(3,6): " << lca.dist(3,6) << "\n"; // 4
    return 0;
}
