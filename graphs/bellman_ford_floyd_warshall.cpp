// Bellman-Ford (negative weights, detects negative cycles)
// Floyd-Warshall (all-pairs shortest paths)
#include <iostream>
#include <vector>
#include <climits>
#include <tuple>

// ── Bellman-Ford ──────────────────────────────────────────────────────────────
// O(V * E), handles negative weights, detects negative cycles
struct BFEdge { int u, v, w; };

struct BellmanFordResult {
    std::vector<int> dist;
    bool hasNegativeCycle;
};

BellmanFordResult bellmanFord(int V, const std::vector<BFEdge>& edges, int src) {
    std::vector<int> dist(V, INT_MAX);
    dist[src] = 0;

    // Relax all edges V-1 times
    for (int i = 1; i < V; ++i) {
        for (const auto& [u, v, w] : edges) {
            if (dist[u] != INT_MAX && dist[u] + w < dist[v])
                dist[v] = dist[u] + w;
        }
    }

    // Check for negative cycles (V-th relaxation)
    bool hasNegCycle = false;
    for (const auto& [u, v, w] : edges) {
        if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
            hasNegCycle = true;
            break;
        }
    }
    return {dist, hasNegCycle};
}

// SPFA (Shortest Path Faster Algorithm) — optimized Bellman-Ford
#include <queue>
std::vector<int> spfa(int V, const std::vector<std::vector<std::pair<int,int>>>& adj, int src) {
    std::vector<int> dist(V, INT_MAX), inQueue(V, false), cnt(V, 0);
    dist[src] = 0;
    std::queue<int> q;
    q.push(src); inQueue[src] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop(); inQueue[u] = false;
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (!inQueue[v]) { q.push(v); inQueue[v] = true; }
            }
        }
    }
    return dist;
}

// ── Floyd-Warshall ────────────────────────────────────────────────────────────
// O(V^3), all-pairs shortest paths, detects negative cycles
const int INF = INT_MAX / 2;

std::vector<std::vector<int>> floydWarshall(int V, std::vector<std::vector<int>> dist) {
    // dist[i][j] = weight of edge i->j, INF if no edge, 0 if i==j
    for (int k = 0; k < V; ++k)
        for (int i = 0; i < V; ++i)
            for (int j = 0; j < V; ++j)
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
    return dist;
}

// Reconstruct path using next[][] matrix
std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>>
floydWarshallWithPath(int V, std::vector<std::vector<int>> dist) {
    std::vector<std::vector<int>> next(V, std::vector<int>(V, -1));
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            if (dist[i][j] != INF && i != j) next[i][j] = j;

    for (int k = 0; k < V; ++k)
        for (int i = 0; i < V; ++i)
            for (int j = 0; j < V; ++j)
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
    return {dist, next};
}

std::vector<int> reconstructPath(const std::vector<std::vector<int>>& next, int u, int v) {
    if (next[u][v] == -1) return {};
    std::vector<int> path = {u};
    while (u != v) { u = next[u][v]; path.push_back(u); }
    return path;
}

int main() {
    // Bellman-Ford
    std::vector<BFEdge> edges = {{0,1,-1},{0,2,4},{1,2,3},{1,3,2},{1,4,2},{3,2,5},{3,1,1},{4,3,-3}};
    auto [dist, negCycle] = bellmanFord(5, edges, 0);
    std::cout << "Bellman-Ford from 0:\n";
    for (int i = 0; i < 5; ++i)
        std::cout << "  to " << i << ": " << dist[i] << "\n";
    std::cout << "Negative cycle: " << negCycle << "\n";

    // Floyd-Warshall
    int V = 4;
    std::vector<std::vector<int>> d = {
        {0,   3,   INF, 7},
        {8,   0,   2,   INF},
        {5,   INF, 0,   1},
        {2,   INF, INF, 0}
    };
    auto result = floydWarshall(V, d);
    std::cout << "\nFloyd-Warshall all-pairs:\n";
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j)
            std::cout << (result[i][j] == INF ? -1 : result[i][j]) << "\t";
        std::cout << "\n";
    }
    return 0;
}
