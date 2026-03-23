// Dijkstra's Algorithm — Single Source Shortest Path (non-negative weights)
// O((V + E) log V) with priority queue
#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using Edge = std::pair<int,int>; // {weight, vertex}

class DijkstraGraph {
public:
    int V;
    std::vector<std::vector<Edge>> adj;

    DijkstraGraph(int v) : V(v), adj(v) {}

    void addEdge(int u, int v, int w) {
        adj[u].push_back({w, v});
        adj[v].push_back({w, u}); // undirected
    }

    // Returns {dist, parent} arrays
    std::pair<std::vector<int>, std::vector<int>> dijkstra(int src) const {
        std::vector<int> dist(V, INT_MAX), parent(V, -1);
        std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> pq;
        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue; // stale entry
            for (auto [w, v] : adj[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }
        return {dist, parent};
    }

    // Reconstruct path from src to dst
    std::vector<int> getPath(const std::vector<int>& parent, int dst) const {
        std::vector<int> path;
        for (int v = dst; v != -1; v = parent[v]) path.push_back(v);
        std::reverse(path.begin(), path.end());
        return path;
    }

    // Dijkstra on directed weighted graph
    std::vector<int> dijkstraDirected(int src) const {
        std::vector<int> dist(V, INT_MAX);
        std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> pq;
        dist[src] = 0;
        pq.push({0, src});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (auto [w, v] : adj[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    }

    // All-pairs shortest paths using Dijkstra (for sparse graphs)
    std::vector<std::vector<int>> allPairsDijkstra() const {
        std::vector<std::vector<int>> result(V);
        for (int i = 0; i < V; ++i)
            result[i] = dijkstraDirected(i);
        return result;
    }
};

int main() {
    DijkstraGraph g(9);
    g.addEdge(0, 1, 4);
    g.addEdge(0, 7, 8);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 7, 11);
    g.addEdge(2, 3, 7);
    g.addEdge(2, 5, 4);
    g.addEdge(2, 8, 2);
    g.addEdge(3, 4, 9);
    g.addEdge(3, 5, 14);
    g.addEdge(4, 5, 10);
    g.addEdge(5, 6, 2);
    g.addEdge(6, 7, 1);
    g.addEdge(6, 8, 6);
    g.addEdge(7, 8, 7);

    auto [dist, parent] = g.dijkstra(0);
    std::cout << "Shortest distances from 0:\n";
    for (int i = 0; i < 9; ++i)
        std::cout << "  to " << i << ": " << dist[i] << "\n";

    auto path = g.getPath(parent, 4);
    std::cout << "Path 0->4: ";
    for (int x : path) std::cout << x << " ";
    std::cout << "\n";
    return 0;
}
