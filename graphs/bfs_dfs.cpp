// Graph BFS and DFS — adjacency list representation
// Undirected and directed graphs
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <functional>

class Graph {
public:
    int V;
    std::vector<std::vector<int>> adj;
    bool directed;

    Graph(int v, bool dir = false) : V(v), adj(v), directed(dir) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        if (!directed) adj[v].push_back(u);
    }

    // ── BFS ───────────────────────────────────────────────────────────────────
    std::vector<int> bfs(int start) const {
        std::vector<bool> visited(V, false);
        std::vector<int> order;
        std::queue<int> q;
        visited[start] = true;
        q.push(start);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int v : adj[u]) {
                if (!visited[v]) { visited[v] = true; q.push(v); }
            }
        }
        return order;
    }

    // BFS shortest path (unweighted)
    std::vector<int> shortestPath(int src, int dst) const {
        std::vector<int> dist(V, -1), parent(V, -1);
        std::queue<int> q;
        dist[src] = 0; q.push(src);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
        if (dist[dst] == -1) return {};
        std::vector<int> path;
        for (int v = dst; v != -1; v = parent[v]) path.push_back(v);
        std::reverse(path.begin(), path.end());
        return path;
    }

    // ── DFS ───────────────────────────────────────────────────────────────────
    void dfsHelper(int u, std::vector<bool>& visited, std::vector<int>& order) const {
        visited[u] = true;
        order.push_back(u);
        for (int v : adj[u])
            if (!visited[v]) dfsHelper(v, visited, order);
    }

    std::vector<int> dfs(int start) const {
        std::vector<bool> visited(V, false);
        std::vector<int> order;
        dfsHelper(start, visited, order);
        return order;
    }

    // Iterative DFS using explicit stack
    std::vector<int> dfsIterative(int start) const {
        std::vector<bool> visited(V, false);
        std::vector<int> order;
        std::stack<int> st;
        st.push(start);
        while (!st.empty()) {
            int u = st.top(); st.pop();
            if (visited[u]) continue;
            visited[u] = true;
            order.push_back(u);
            for (auto it = adj[u].rbegin(); it != adj[u].rend(); ++it)
                if (!visited[*it]) st.push(*it);
        }
        return order;
    }

    // ── Connected Components (undirected) ─────────────────────────────────────
    int countComponents() const {
        std::vector<bool> visited(V, false);
        int count = 0;
        for (int i = 0; i < V; ++i) {
            if (!visited[i]) {
                ++count;
                std::vector<int> tmp;
                dfsHelper(i, visited, tmp);
            }
        }
        return count;
    }

    // ── Cycle Detection ───────────────────────────────────────────────────────
    bool hasCycleUndirected(int u, int parent, std::vector<bool>& vis) const {
        vis[u] = true;
        for (int v : adj[u]) {
            if (!vis[v]) { if (hasCycleUndirected(v, u, vis)) return true; }
            else if (v != parent) return true;
        }
        return false;
    }

    bool hasCycleUndirected() const {
        std::vector<bool> vis(V, false);
        for (int i = 0; i < V; ++i)
            if (!vis[i] && hasCycleUndirected(i, -1, vis)) return true;
        return false;
    }

    bool hasCycleDirected(int u, std::vector<int>& color) const {
        color[u] = 1; // in stack
        for (int v : adj[u]) {
            if (color[v] == 1) return true;
            if (color[v] == 0 && hasCycleDirected(v, color)) return true;
        }
        color[u] = 2; // done
        return false;
    }

    bool hasCycleDirected() const {
        std::vector<int> color(V, 0); // 0=white, 1=gray, 2=black
        for (int i = 0; i < V; ++i)
            if (color[i] == 0 && hasCycleDirected(i, color)) return true;
        return false;
    }

    // ── Bipartite Check ───────────────────────────────────────────────────────
    bool isBipartite() const {
        std::vector<int> color(V, -1);
        for (int start = 0; start < V; ++start) {
            if (color[start] != -1) continue;
            std::queue<int> q;
            color[start] = 0; q.push(start);
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : adj[u]) {
                    if (color[v] == -1) { color[v] = 1 - color[u]; q.push(v); }
                    else if (color[v] == color[u]) return false;
                }
            }
        }
        return true;
    }
};

void print(const std::vector<int>& v) {
    for (int x : v) std::cout << x << " "; std::cout << "\n";
}

int main() {
    Graph g(6);
    g.addEdge(0, 1); g.addEdge(0, 2);
    g.addEdge(1, 3); g.addEdge(2, 4);
    g.addEdge(3, 5); g.addEdge(4, 5);

    std::cout << "BFS from 0: "; print(g.bfs(0));
    std::cout << "DFS from 0: "; print(g.dfs(0));
    std::cout << "DFS iter:   "; print(g.dfsIterative(0));

    auto path = g.shortestPath(0, 5);
    std::cout << "Shortest path 0->5: "; print(path);
    std::cout << "Bipartite: " << g.isBipartite() << "\n"; // 1
    std::cout << "Has cycle: " << g.hasCycleUndirected() << "\n"; // 1

    Graph dg(4, true);
    dg.addEdge(0, 1); dg.addEdge(1, 2); dg.addEdge(2, 0); // cycle
    std::cout << "Directed cycle: " << dg.hasCycleDirected() << "\n"; // 1
    return 0;
}
