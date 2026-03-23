// Topological Sort — Kahn's (BFS) and DFS-based
// Only for Directed Acyclic Graphs (DAGs)
#include <iostream>
#include <vector>
#include <queue>
#include <stack>

class TopoGraph {
public:
    int V;
    std::vector<std::vector<int>> adj;

    TopoGraph(int v) : V(v), adj(v) {}
    void addEdge(int u, int v) { adj[u].push_back(v); }

    // ── Kahn's Algorithm (BFS-based) ──────────────────────────────────────────
    // Returns topological order, or empty if cycle exists
    std::vector<int> kahnTopoSort() const {
        std::vector<int> inDegree(V, 0);
        for (int u = 0; u < V; ++u)
            for (int v : adj[u]) ++inDegree[v];

        std::queue<int> q;
        for (int i = 0; i < V; ++i)
            if (inDegree[i] == 0) q.push(i);

        std::vector<int> order;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int v : adj[u])
                if (--inDegree[v] == 0) q.push(v);
        }
        return (order.size() == (size_t)V) ? order : std::vector<int>{}; // empty = has cycle
    }

    // ── DFS-based Topological Sort ────────────────────────────────────────────
    void dfsHelper(int u, std::vector<bool>& visited, std::stack<int>& st) const {
        visited[u] = true;
        for (int v : adj[u])
            if (!visited[v]) dfsHelper(v, visited, st);
        st.push(u);
    }

    std::vector<int> dfsTopoSort() const {
        std::vector<bool> visited(V, false);
        std::stack<int> st;
        for (int i = 0; i < V; ++i)
            if (!visited[i]) dfsHelper(i, visited, st);
        std::vector<int> order;
        while (!st.empty()) { order.push_back(st.top()); st.pop(); }
        return order;
    }

    // ── Shortest Path in DAG (using topo sort + relaxation) ───────────────────
    std::vector<int> shortestPathDAG(int src, const std::vector<std::vector<std::pair<int,int>>>& wadj) const {
        auto order = dfsTopoSort();
        std::vector<int> dist(V, INT_MAX);
        dist[src] = 0;
        for (int u : order) {
            if (dist[u] != INT_MAX) {
                for (auto [v, w] : wadj[u])
                    if (dist[u] + w < dist[v]) dist[v] = dist[u] + w;
            }
        }
        return dist;
    }

    // ── All Topological Sorts ─────────────────────────────────────────────────
    void allTopoSorts(std::vector<int>& inDegree, std::vector<bool>& visited,
                      std::vector<int>& current, std::vector<std::vector<int>>& result) const {
        bool found = false;
        for (int i = 0; i < V; ++i) {
            if (inDegree[i] == 0 && !visited[i]) {
                visited[i] = true;
                current.push_back(i);
                for (int v : adj[i]) --inDegree[v];
                allTopoSorts(inDegree, visited, current, result);
                visited[i] = false;
                current.pop_back();
                for (int v : adj[i]) ++inDegree[v];
                found = true;
            }
        }
        if (!found) result.push_back(current);
    }

    std::vector<std::vector<int>> getAllTopoSorts() const {
        std::vector<int> inDegree(V, 0);
        for (int u = 0; u < V; ++u)
            for (int v : adj[u]) ++inDegree[v];
        std::vector<bool> visited(V, false);
        std::vector<int> current;
        std::vector<std::vector<int>> result;
        allTopoSorts(inDegree, visited, current, result);
        return result;
    }
};

void print(const std::vector<int>& v) {
    for (int x : v) std::cout << x << " "; std::cout << "\n";
}

int main() {
    TopoGraph g(6);
    g.addEdge(5, 2); g.addEdge(5, 0);
    g.addEdge(4, 0); g.addEdge(4, 1);
    g.addEdge(2, 3); g.addEdge(3, 1);

    auto kahn = g.kahnTopoSort();
    std::cout << "Kahn's topo sort: "; print(kahn);

    auto dfs = g.dfsTopoSort();
    std::cout << "DFS topo sort:    "; print(dfs);

    // Course schedule problem
    // Can finish all courses?
    TopoGraph courses(4);
    courses.addEdge(1, 0); courses.addEdge(2, 0); courses.addEdge(3, 1); courses.addEdge(3, 2);
    auto order = courses.kahnTopoSort();
    std::cout << "Can finish courses: " << (!order.empty() ? "YES" : "NO (cycle)") << "\n";
    if (!order.empty()) { std::cout << "Order: "; print(order); }
    return 0;
}
