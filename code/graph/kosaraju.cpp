#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// Kosaraju's Algorithm (SCCs & Condensed DAG)

// Finds Strongly Connected Components in a directed graph
// and builds a condensed DAG where each node is an SCC.

// 1-indexed implementation.
// Time Complexity: O(V + E)
// Space Complexity: O(V + E)

const int MAXN = 1e5 + 5;
vector<int> adj[MAXN], adjr[MAXN], dag[MAXN];
vector<int> comp, ord;
vector<bool> vis;
int n, m;
int curr=0;

void dfs(int u=1) {
    vis[u] = true;
    for (auto &v : adj[u]) {
        if (vis[v]) continue;
        dfs(v);
    }
    ord.push_back(u);
}

void dfsr(int u) {
    comp[u] = curr;
    for (auto &v : adjr[u]) {
        if (~comp[v]) continue;
        dfsr(v);
    }
}

void kosaraju() {
    ord.clear();
    comp = vector<int>(n+1, -1);
    vis = vector<bool>(n+1, false);
    curr = 0;

    for (int i = 0; i <= n; i++)
        dag[i].clear();

    for (int u=1; u <= n; u++) {
        if (vis[u]) continue;
        dfs(u);
    }
    reverse(ord.begin(), ord.end());

    for (auto &u : ord) {
        if (~comp[u]) continue;
        dfsr(u), curr++;
    }

    vector<bool> flag(curr, false);

    for (int u=1; u <= n; u++) {
        for (auto &v : adj[u]) {
            if (comp[v] == comp[u] or flag[comp[v]]) continue;

            flag[comp[v]] = true;
            dag[comp[u]].push_back(comp[v]);
        }
        for (auto &v : adj[u])
            flag[comp[v]] = false;
    }
}