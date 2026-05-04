#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// Tarjan's Algorithm (Bridges & Articulation Points)

// Finds all bridges and cut vertices in an UNDIRECTED graph.
// NOTE: Assumes a simple graph (no parallel edges).

// Time Complexity: O(V + E)
// Space Complexity: O(V + E)

const int MAXN = 1e5 + 5;
int tin[MAXN], low[MAXN], timer=0;
vector<int> adj[MAXN];
int n;

vector<pair<int, int>> bridges;
vector<int> articulation_points;

void tarjan(int u, int p = -1) {
    low[u] = tin[u] = timer++;

    int flag = false, children = 0;

    for (auto &v : adj[u]) {
        if (v == p) continue;
        
        if (~tin[v]) {
            low[u] = min(low[u], tin[v]);
        } else {
            tarjan(v, u);

            low[u] = min(low[v], low[u]);

            if (low[v] > tin[u]) bridges.push_back({u, v});
            if (low[v] >= tin[u]) flag = true;
            children++;
        }
    }

    if (p == -1 && children >= 2) articulation_points.push_back(u);
    if (~p && flag) articulation_points.push_back(u);
}

void find_cut_elements() {
    timer = 0;
    bridges.clear();
    articulation_points.clear();
    memset(tin, -1, sizeof(tin));

    for (int i = 0; i < n; i++)
        if (!~tin[i]) tarjan(i);
}