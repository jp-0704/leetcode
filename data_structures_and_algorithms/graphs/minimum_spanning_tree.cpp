// Minimum Spanning Tree — Kruskal's and Prim's algorithms
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>

// ── Kruskal's Algorithm (Union-Find) ─────────────────────────────────────────
// O(E log E), sorts edges then greedily picks minimum
struct Edge { int u, v, w; };

class UnionFind {
    std::vector<int> parent, rank_;
public:
    UnionFind(int n) : parent(n), rank_(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]); // path compression
        return parent[x];
    }
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;
        if (rank_[px] < rank_[py]) std::swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) ++rank_[px];
        return true;
    }
    bool connected(int x, int y) { return find(x) == find(y); }
};

struct MSTResult { std::vector<Edge> edges; int totalWeight; };

MSTResult kruskal(int V, std::vector<Edge> edges) {
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b){ return a.w < b.w; });
    UnionFind uf(V);
    MSTResult result{{}, 0};
    for (auto& e : edges) {
        if (uf.unite(e.u, e.v)) {
            result.edges.push_back(e);
            result.totalWeight += e.w;
            if ((int)result.edges.size() == V - 1) break;
        }
    }
    return result;
}

// ── Prim's Algorithm ──────────────────────────────────────────────────────────
// O((V + E) log V) with priority queue
using WEdge = std::pair<int,int>; // {weight, vertex}

MSTResult prim(int V, const std::vector<std::vector<WEdge>>& adj, int src = 0) {
    std::vector<int> key(V, INT_MAX), parent(V, -1);
    std::vector<bool> inMST(V, false);
    std::priority_queue<WEdge, std::vector<WEdge>, std::greater<WEdge>> pq;

    key[src] = 0;
    pq.push({0, src});

    MSTResult result{{}, 0};
    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop();
        if (inMST[u]) continue;
        inMST[u] = true;
        result.totalWeight += w;
        if (parent[u] != -1) result.edges.push_back({parent[u], u, w});

        for (auto [ew, v] : adj[u]) {
            if (!inMST[v] && ew < key[v]) {
                key[v] = ew;
                parent[v] = u;
                pq.push({ew, v});
            }
        }
    }
    return result;
}

// ── Borůvka's Algorithm ───────────────────────────────────────────────────────
// O(E log V), good for parallel computation
int boruvka(int V, std::vector<Edge> edges) {
    UnionFind uf(V);
    int totalWeight = 0, components = V;
    while (components > 1) {
        std::vector<int> cheapest(V, -1);
        for (int i = 0; i < (int)edges.size(); ++i) {
            int pu = uf.find(edges[i].u), pv = uf.find(edges[i].v);
            if (pu == pv) continue;
            if (cheapest[pu] == -1 || edges[i].w < edges[cheapest[pu]].w) cheapest[pu] = i;
            if (cheapest[pv] == -1 || edges[i].w < edges[cheapest[pv]].w) cheapest[pv] = i;
        }
        for (int v = 0; v < V; ++v) {
            if (cheapest[v] != -1) {
                auto& e = edges[cheapest[v]];
                if (uf.unite(e.u, e.v)) { totalWeight += e.w; --components; }
            }
        }
    }
    return totalWeight;
}

int main() {
    int V = 4;
    std::vector<Edge> edges = {{0,1,10},{0,2,6},{0,3,5},{1,3,15},{2,3,4}};

    auto kruskalMST = kruskal(V, edges);
    std::cout << "Kruskal MST weight: " << kruskalMST.totalWeight << "\n"; // 19
    for (auto& e : kruskalMST.edges)
        std::cout << "  " << e.u << " -- " << e.v << " = " << e.w << "\n";

    // Build adjacency list for Prim
    std::vector<std::vector<WEdge>> adj(V);
    for (auto& e : edges) {
        adj[e.u].push_back({e.w, e.v});
        adj[e.v].push_back({e.w, e.u});
    }
    auto primMST = prim(V, adj);
    std::cout << "\nPrim MST weight: " << primMST.totalWeight << "\n"; // 19
    for (auto& e : primMST.edges)
        std::cout << "  " << e.u << " -- " << e.v << " = " << e.w << "\n";

    std::cout << "\nBoruvka MST weight: " << boruvka(V, edges) << "\n"; // 19
    return 0;
}
