// Network Flow — Ford-Fulkerson, Edmonds-Karp, Dinic's Algorithm
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

// ── Ford-Fulkerson with BFS (Edmonds-Karp) — O(VE^2) ────────────────────────
struct FlowGraph {
    int V;
    std::vector<std::vector<int>> cap; // capacity matrix
    std::vector<std::vector<int>> adj;

    FlowGraph(int v) : V(v), cap(v, std::vector<int>(v, 0)), adj(v) {}

    void addEdge(int u, int v, int c) {
        cap[u][v] += c;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    bool bfs(int s, int t, std::vector<int>& parent) {
        std::vector<bool> visited(V, false);
        std::queue<int> q;
        q.push(s); visited[s] = true; parent[s] = -1;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (!visited[v] && cap[u][v] > 0) {
                    parent[v] = u; visited[v] = true;
                    if (v == t) return true;
                    q.push(v);
                }
            }
        }
        return false;
    }

    int maxFlow(int s, int t) {
        std::vector<int> parent(V);
        int flow = 0;
        while (bfs(s, t, parent)) {
            // Find min capacity along the path
            int pathFlow = INT_MAX;
            for (int v = t; v != s; v = parent[v])
                pathFlow = std::min(pathFlow, cap[parent[v]][v]);
            // Update capacities (residual graph)
            for (int v = t; v != s; v = parent[v]) {
                cap[parent[v]][v] -= pathFlow;
                cap[v][parent[v]] += pathFlow;
            }
            flow += pathFlow;
        }
        return flow;
    }

    // Min cut — returns set of nodes reachable from s after max flow
    std::vector<bool> minCut(int s, int t) {
        maxFlow(s, t);
        std::vector<bool> visited(V, false);
        std::queue<int> q;
        q.push(s); visited[s] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u])
                if (!visited[v] && cap[u][v] > 0) { visited[v] = true; q.push(v); }
        }
        return visited;
    }
};

// ── Dinic's Algorithm — O(V^2 * E), much faster in practice ─────────────────
struct DinicGraph {
    struct Edge { int to, rev; int cap; };
    int V;
    std::vector<std::vector<Edge>> graph;
    std::vector<int> level, iter;

    DinicGraph(int v) : V(v), graph(v), level(v), iter(v) {}

    void addEdge(int u, int v, int cap) {
        graph[u].push_back({v, (int)graph[v].size(), cap});
        graph[v].push_back({u, (int)graph[u].size()-1, 0}); // reverse edge
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        std::queue<int> q;
        level[s] = 0; q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& e : graph[u])
                if (e.cap > 0 && level[e.to] < 0) { level[e.to] = level[u]+1; q.push(e.to); }
        }
        return level[t] >= 0;
    }

    int dfs(int u, int t, int pushed) {
        if (u == t) return pushed;
        for (int& i = iter[u]; i < (int)graph[u].size(); ++i) {
            auto& e = graph[u][i];
            if (e.cap > 0 && level[u]+1 == level[e.to]) {
                int d = dfs(e.to, t, std::min(pushed, e.cap));
                if (d > 0) { e.cap -= d; graph[e.to][e.rev].cap += d; return d; }
            }
        }
        return 0;
    }

    long long maxFlow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            fill(iter.begin(), iter.end(), 0);
            int d;
            while ((d = dfs(s, t, INT_MAX)) > 0) flow += d;
        }
        return flow;
    }
};

// ── Bipartite Matching (Max Matching using augmenting paths) ─────────────────
class BipartiteMatching {
    int L, R;
    std::vector<std::vector<int>> adj; // adj[u] = list of right nodes
    std::vector<int> matchL, matchR;
    std::vector<bool> visited;

    bool dfs(int u) {
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                if (matchR[v] == -1 || dfs(matchR[v])) {
                    matchL[u] = v; matchR[v] = u; return true;
                }
            }
        }
        return false;
    }

public:
    BipartiteMatching(int l, int r) : L(l), R(r), adj(l), matchL(l,-1), matchR(r,-1), visited(r) {}

    void addEdge(int u, int v) { adj[u].push_back(v); }

    int maxMatching() {
        int matching = 0;
        for (int u = 0; u < L; ++u) {
            fill(visited.begin(), visited.end(), false);
            if (dfs(u)) ++matching;
        }
        return matching;
    }
};

int main() {
    // Edmonds-Karp
    FlowGraph g(6);
    g.addEdge(0,1,16); g.addEdge(0,2,13);
    g.addEdge(1,2,10); g.addEdge(1,3,12);
    g.addEdge(2,1,4);  g.addEdge(2,4,14);
    g.addEdge(3,2,9);  g.addEdge(3,5,20);
    g.addEdge(4,3,7);  g.addEdge(4,5,4);
    std::cout << "Max flow (Edmonds-Karp): " << g.maxFlow(0, 5) << "\n"; // 23

    // Dinic's
    DinicGraph dg(6);
    dg.addEdge(0,1,16); dg.addEdge(0,2,13);
    dg.addEdge(1,2,10); dg.addEdge(1,3,12);
    dg.addEdge(2,1,4);  dg.addEdge(2,4,14);
    dg.addEdge(3,2,9);  dg.addEdge(3,5,20);
    dg.addEdge(4,3,7);  dg.addEdge(4,5,4);
    std::cout << "Max flow (Dinic's): " << dg.maxFlow(0, 5) << "\n"; // 23

    // Bipartite matching
    BipartiteMatching bm(4, 4);
    bm.addEdge(0,0); bm.addEdge(0,1); bm.addEdge(1,2); bm.addEdge(2,2);
    bm.addEdge(2,3); bm.addEdge(3,3);
    std::cout << "Max bipartite matching: " << bm.maxMatching() << "\n"; // 4
    return 0;
}
