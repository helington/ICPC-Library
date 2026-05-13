#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// Tarjan's Algorithm (Bridges & Articulation Points)

// Finds all bridges and articulation points (cut vertices) in an UNDIRECTED graph.
// NOTE: Assumes a simple graph (no parallel edges). To support parallel edges, 
// pass the edge ID instead of the parent node to the DFS.

// 1-indexed internally.
// Time Complexity: O(V + E)
// Space Complexity: O(V + E)

const int MAXN = 1e5 + 5;
vector<int> adj[MAXN];
int n;

vector<pair<int, int>> bridges;
vector<int> art;

void tarjan() {
    vector<int> tin(n+1, -1), low(n+1, -1);
    int timer = 0;
    
    bridges.clear();
    art.clear();

    auto dfs = [&](auto &&dfs, int u, int p = -1) -> void {
        low[u] = tin[u] = timer++;

        bool is_art = false;
        int children = 0;

        for (int &v : adj[u]) {
            if (v == p) continue;
            
            if (~tin[v]) low[u] = min(low[u], tin[v]);
            else {
                dfs(dfs, v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) bridges.push_back({min(u, v), max(u, v)});
                if (low[v] >= tin[u]) is_art = true;
                children++;
            }
        }

        if (p == -1 && children >= 2) art.push_back(u);
        if (~p && is_art) art.push_back(u);
    };

    for (int u = 1; u <= n; u++)
        if (tin[u] == -1) dfs(dfs, u);
}