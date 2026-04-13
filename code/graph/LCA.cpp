#include <bits/stdc++.h>

using namespace std;

// Lowest Common Ancestor (Binary Lifting)

// Uses Euler Tour (tin/tout) to check ancestors in O(1).

// Time Complexity:
//   - Build (dfs): O(N log N)
//   - LCA Query: O(log N)
//   - K-th Ancestor Query: O(log N)
// Space Complexity: O(N log N)

const int MAXN = 2e5 + 10;
const int LOG = 20;

int timer = 0;
int tin[MAXN], tout[MAXN];
int up[MAXN][LOG];
vector<int> adj[MAXN];

// Call dfs(root, root) to initialize! 
void dfs(int u, int p) {
    tin[u] = ++timer;
    up[u][0] = p;

    for (int i = 1; i < LOG; i++)
        up[u][i] = up[up[u][i - 1]][i - 1];

    for (int v : adj[u])
        if (v != p) dfs(v, u);

    tout[u] = ++timer;
}

int kth_anc(int u, int k) {
    for (int i = 0; i < LOG; i++)
        if ((1 << i) & k) u = up[u][i];
    return u;
}

bool is_ancestor(int u, int v) { return tin[u] <= tin[v] && tout[u] >= tout[v]; }

int lca(int u, int v) {
    if (is_ancestor(u, v)) return u;
    if (is_ancestor(v, u)) return v;

    for (int i = LOG - 1; i >= 0; i--)
        if (!is_ancestor(up[u][i], v))
            u = up[u][i];
    
    return up[u][0];
}