#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// Dinic's Algorithm (Maximum Flow)

// Efficient algorithm to compute the maximum flow in a flow network.
// Uses level graphs and dead-end pruning for fast execution.

// 0-indexed or 1-indexed supported (allocated for sizes up to N).
// Time Complexity: O(V^2 * E) for general graphs, O(E * sqrt(V)) for bipartite matching.
// Space Complexity: O(V + E)

const ll FLOW_INF = 1e18;

struct Edge {
    int u, v;
    ll cap;
    Edge(int u, int v, ll cap) : u(u), v(v), cap(cap) {}
};

struct Dinic {
    int N, m = 0;
    int s, t;
    vector<vector<int>> adj;
    vector<Edge> edges;
    vector<int> lvl, ptr;

    Dinic(int n, int s, int t) : N(n+1), s(s), t(t), adj(N), lvl(N), ptr(N) {}

    void add_edge(int u, int v, ll cap) {
        adj[u].push_back(m);
        edges.emplace_back(u, v, cap);

        adj[v].push_back(m + 1);
        edges.emplace_back(v, u, 0);

        m += 2;
    }

    bool bfs() {
        fill(lvl.begin(), lvl.end(), -1);
        lvl[s] = 0;

        queue<int> q;
        q.push(s);

        while (!q.empty()) {
            int u = q.front(); q.pop();

            for (int i : adj[u]) {
                int v = edges[i].v;

                if (edges[i].cap == 0 or lvl[v] != -1) continue;

                lvl[v] = lvl[u] + 1;
                q.push(v);
            }
        }

        return lvl[t] != -1;
    }

    ll dfs(int u, ll flow) {
        if (flow == 0) return 0;
        if (u == t) return flow;

        for (int &i = ptr[u]; i < adj[u].size(); i++) {
            int at = adj[u][i];
            int v = edges[at].v;

            if (lvl[v] != lvl[u] + 1) continue;

            if (ll got = dfs(v, min(flow, edges[at].cap))) {
                edges[at].cap -= got;
                edges[at^1].cap += got;
                return got;
            }
        }

        return 0;
    }

    ll max_flow() {
        ll ans = 0;

        while (bfs()) {
            fill(ptr.begin(), ptr.end(), 0);
            while (ll got = dfs(s, FLOW_INF))
                ans += got;
        }

        return ans;
    }
};