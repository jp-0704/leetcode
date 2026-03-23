// Strongly Connected Components (Kosaraju, Tarjan)
// Articulation Points and Bridges
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

// ── Kosaraju's SCC ────────────────────────────────────────────────────────────
class Kosaraju {
    int V;
    std::vector<std::vector<int>> adj, radj;

    void dfs1(int u, std::vector<bool>& vis, std::stack<int>& st) {
        vis[u] = true;
        for (int v : adj[u]) if (!vis[v]) dfs1(v, vis, st);
        st.push(u);
    }

    void dfs2(int u, std::vector<bool>& vis, std::vector<int>& comp) {
        vis[u] = true; comp.push_back(u);
        for (int v : radj[u]) if (!vis[v]) dfs2(v, vis, comp);
    }

public:
    Kosaraju(int v) : V(v), adj(v), radj(v) {}

    void addEdge(int u, int v) { adj[u].push_back(v); radj[v].push_back(u); }

    std::vector<std::vector<int>> getSCCs() {
        std::stack<int> st;
        std::vector<bool> vis(V, false);
        for (int i = 0; i < V; ++i) if (!vis[i]) dfs1(i, vis, st);
        fill(vis.begin(), vis.end(), false);
        std::vector<std::vector<int>> sccs;
        while (!st.empty()) {
            int u = st.top(); st.pop();
            if (!vis[u]) {
                std::vector<int> comp;
                dfs2(u, vis, comp);
                sccs.push_back(comp);
            }
        }
        return sccs;
    }
};

// ── Tarjan's SCC ──────────────────────────────────────────────────────────────
class Tarjan {
    int V, timer;
    std::vector<std::vector<int>> adj;
    std::vector<int> disc, low, comp;
    std::vector<bool> onStack;
    std::stack<int> st;
    std::vector<std::vector<int>> sccs;

    void dfs(int u) {
        disc[u] = low[u] = timer++;
        st.push(u); onStack[u] = true;
        for (int v : adj[u]) {
            if (disc[v] == -1) { dfs(v); low[u] = std::min(low[u], low[v]); }
            else if (onStack[v]) low[u] = std::min(low[u], disc[v]);
        }
        if (low[u] == disc[u]) {
            std::vector<int> scc;
            while (true) {
                int v = st.top(); st.pop();
                onStack[v] = false;
                scc.push_back(v);
                if (v == u) break;
            }
            sccs.push_back(scc);
        }
    }

public:
    Tarjan(int v) : V(v), timer(0), adj(v), disc(v,-1), low(v), comp(v,-1), onStack(v,false) {}

    void addEdge(int u, int v) { adj[u].push_back(v); }

    std::vector<std::vector<int>> getSCCs() {
        for (int i = 0; i < V; ++i) if (disc[i] == -1) dfs(i);
        return sccs;
    }
};

// ── Articulation Points and Bridges ──────────────────────────────────────────
class ArticulationGraph {
    int V, timer;
    std::vector<std::vector<int>> adj;
    std::vector<int> disc, low, parent;
    std::vector<bool> ap;

    void dfsAP(int u) {
        int children = 0;
        disc[u] = low[u] = timer++;
        for (int v : adj[u]) {
            if (disc[v] == -1) {
                ++children;
                parent[v] = u;
                dfsAP(v);
                low[u] = std::min(low[u], low[v]);
                if (parent[u] == -1 && children > 1) ap[u] = true;
                if (parent[u] != -1 && low[v] >= disc[u]) ap[u] = true;
            } else if (v != parent[u]) {
                low[u] = std::min(low[u], disc[v]);
            }
        }
    }

public:
    ArticulationGraph(int v) : V(v), timer(0), adj(v), disc(v,-1), low(v), parent(v,-1), ap(v,false) {}

    void addEdge(int u, int v) { adj[u].push_back(v); adj[v].push_back(u); }

    std::vector<int> getArticulationPoints() {
        for (int i = 0; i < V; ++i) if (disc[i] == -1) dfsAP(i);
        std::vector<int> result;
        for (int i = 0; i < V; ++i) if (ap[i]) result.push_back(i);
        return result;
    }

    // Bridges: edge (u,v) where low[v] > disc[u]
    std::vector<std::pair<int,int>> getBridges() {
        timer = 0;
        fill(disc.begin(), disc.end(), -1);
        fill(parent.begin(), parent.end(), -1);
        for (int i = 0; i < V; ++i) if (disc[i] == -1) dfsAP(i);
        // Recompute for bridges
        std::vector<std::pair<int,int>> bridges;
        std::vector<int> d(V,-1), l(V,0), par(V,-1);
        int t = 0;
        std::function<void(int)> dfs = [&](int u) {
            d[u] = l[u] = t++;
            for (int v : adj[u]) {
                if (d[v] == -1) { par[v] = u; dfs(v); l[u] = std::min(l[u], l[v]);
                    if (l[v] > d[u]) bridges.push_back({u,v}); }
                else if (v != par[u]) l[u] = std::min(l[u], d[v]);
            }
        };
        for (int i = 0; i < V; ++i) if (d[i] == -1) dfs(i);
        return bridges;
    }
};

int main() {
    // Kosaraju
    Kosaraju g(5);
    g.addEdge(1,0); g.addEdge(0,2); g.addEdge(2,1); g.addEdge(0,3); g.addEdge(3,4);
    auto sccs = g.getSCCs();
    std::cout << "Kosaraju SCCs (" << sccs.size() << "):\n";
    for (auto& scc : sccs) {
        std::cout << "  "; for (int x : scc) std::cout << x << " "; std::cout << "\n";
    }

    // Tarjan
    Tarjan tg(4);
    tg.addEdge(0,1); tg.addEdge(1,2); tg.addEdge(2,0); tg.addEdge(1,3);
    auto tsccs = tg.getSCCs();
    std::cout << "\nTarjan SCCs (" << tsccs.size() << "):\n";
    for (auto& scc : tsccs) {
        std::cout << "  "; for (int x : scc) std::cout << x << " "; std::cout << "\n";
    }

    // Articulation Points
    ArticulationGraph ag(5);
    ag.addEdge(1,0); ag.addEdge(0,2); ag.addEdge(2,1); ag.addEdge(0,3); ag.addEdge(3,4);
    auto aps = ag.getArticulationPoints();
    std::cout << "\nArticulation points: ";
    for (int x : aps) std::cout << x << " "; std::cout << "\n";
    return 0;
}
