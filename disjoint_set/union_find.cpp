// Disjoint Set Union (Union-Find) — with Path Compression and Union by Rank/Size
// O(α(n)) per operation (nearly constant)
#include <iostream>
#include <vector>
#include <numeric>
#include <unordered_map>

// ── Basic Union-Find with Path Compression + Union by Rank ───────────────────
class UnionFind {
    std::vector<int> parent, rank_, size_;
    int components;

public:
    UnionFind(int n) : parent(n), rank_(n, 0), size_(n, 1), components(n) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    // Path compression (iterative)
    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]]; // path halving
            x = parent[x];
        }
        return x;
    }

    // Union by rank
    bool unionByRank(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;
        if (rank_[px] < rank_[py]) std::swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) ++rank_[px];
        size_[px] += size_[py];
        --components;
        return true;
    }

    // Union by size
    bool unionBySize(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;
        if (size_[px] < size_[py]) std::swap(px, py);
        parent[py] = px;
        size_[px] += size_[py];
        --components;
        return true;
    }

    bool connected(int x, int y) { return find(x) == find(y); }
    int getSize(int x) { return size_[find(x)]; }
    int numComponents() const { return components; }
};

// ── Applications ──────────────────────────────────────────────────────────────

// Number of islands using Union-Find
int numIslands(std::vector<std::vector<char>>& grid) {
    if (grid.empty()) return 0;
    int rows = grid.size(), cols = grid[0].size();
    UnionFind uf(rows * cols);
    int water = 0;
    auto idx = [&](int r, int c) { return r * cols + c; };

    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            if (grid[r][c] == '0') ++water;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == '1') {
                if (r+1 < rows && grid[r+1][c] == '1') uf.unionByRank(idx(r,c), idx(r+1,c));
                if (c+1 < cols && grid[r][c+1] == '1') uf.unionByRank(idx(r,c), idx(r,c+1));
            }
        }
    }
    // Count unique land roots
    std::unordered_map<int,bool> roots;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            if (grid[r][c] == '1') roots[uf.find(idx(r,c))] = true;
    return roots.size();
}

// Detect cycle in undirected graph
bool detectCycle(int V, const std::vector<std::pair<int,int>>& edges) {
    UnionFind uf(V);
    for (auto [u, v] : edges)
        if (!uf.unionByRank(u, v)) return true;
    return false;
}

// Kruskal's MST (simplified)
#include <algorithm>
struct Edge { int u, v, w; };
int kruskalMST(int V, std::vector<Edge> edges) {
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b){ return a.w < b.w; });
    UnionFind uf(V);
    int total = 0;
    for (auto& e : edges)
        if (uf.unionByRank(e.u, e.v)) total += e.w;
    return total;
}

// Accounts merge (grouping by common email)
#include <string>
std::vector<std::vector<std::string>> accountsMerge(
    const std::vector<std::vector<std::string>>& accounts) {
    std::unordered_map<std::string, int> emailToId;
    std::unordered_map<std::string, std::string> emailToName;
    int id = 0;
    for (const auto& acc : accounts) {
        const std::string& name = acc[0];
        for (int i = 1; i < (int)acc.size(); ++i) {
            if (!emailToId.count(acc[i])) { emailToId[acc[i]] = id++; emailToName[acc[i]] = name; }
        }
    }
    UnionFind uf(id);
    for (const auto& acc : accounts) {
        for (int i = 2; i < (int)acc.size(); ++i)
            uf.unionByRank(emailToId[acc[1]], emailToId[acc[i]]);
    }
    std::unordered_map<int, std::vector<std::string>> groups;
    for (auto& [email, eid] : emailToId)
        groups[uf.find(eid)].push_back(email);
    std::vector<std::vector<std::string>> result;
    for (auto& [root, emails] : groups) {
        std::sort(emails.begin(), emails.end());
        emails.insert(emails.begin(), emailToName[emails[0]]);
        result.push_back(emails);
    }
    return result;
}

int main() {
    UnionFind uf(7);
    uf.unionByRank(0, 1); uf.unionByRank(1, 2);
    uf.unionByRank(3, 4); uf.unionByRank(4, 5);
    std::cout << "Components: " << uf.numComponents() << "\n"; // 3
    std::cout << "0 and 2 connected: " << uf.connected(0, 2) << "\n"; // 1
    std::cout << "0 and 3 connected: " << uf.connected(0, 3) << "\n"; // 0
    std::cout << "Size of 0's component: " << uf.getSize(0) << "\n";  // 3

    std::vector<std::vector<char>> grid = {
        {'1','1','1','1','0'},
        {'1','1','0','1','0'},
        {'1','1','0','0','0'},
        {'0','0','0','0','0'}
    };
    std::cout << "Islands: " << numIslands(grid) << "\n"; // 1

    std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,0}};
    std::cout << "Has cycle: " << detectCycle(3, edges) << "\n"; // 1
    return 0;
}
