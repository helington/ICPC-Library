#include <bits/stdc++.h>
#include "../data structures/SegTreeLazy.cpp"
using namespace std;
#define ll long long
#define endl '\n'

// Heavy-Light Decomposition (HLD)

// Decomposes a tree into a set of vertical disjoint paths (heavy chains).
// Allows executing path and subtree queries/updates in O(log^2 N) using a Segment Tree.

// 1-indexed internally by default (supports 0-indexed if root=0 is specified).
// Time Complexity: 
// - Build: O(N)
// - Path Query/Update: O(log^2 N)
// - Subtree Query/Update: O(log N)
// - LCA: O(log N)
// Space Complexity: O(N)

struct HLD {
    int N, timer;
    vector<vector<int>> adj;
    vector<int> sz, parent, tin, nxt;
    SegTree seg;

    HLD(int n)
        : N(n), seg(n+1), adj(n+1), sz(n+1), parent(n+1), tin(n+1), nxt(n+1) {}
    
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void build(int root=1) {
        timer=0;
        dfs(root, root);
        nxt[root]=root;
        hld(root, root);
    }

    void dfs(int u, int p) {
        parent[u] = p; sz[u] = 1;
        
        for (auto &v : adj[u])
            if (v != p) {
                dfs(v, u);
                sz[u] += sz[v];

                if (sz[v] > sz[adj[u][0]] or adj[u][0] == p)
                    swap(v, adj[u][0]);
            }
    }

    void hld(int u, int p) {
        tin[u] = timer++;

        for (auto &v : adj[u]) 
            if (v != p) {
                nxt[v] = (v == adj[u][0] ? nxt[u] : v);
                hld(v, u);
            }
    }

    ll query_path(int u, int v) {
        if (tin[u] < tin[v]) swap(u, v);
        if (nxt[u] == nxt[v]) return seg.query(tin[v], tin[u]);
        return seg.query(tin[nxt[u]], tin[u]) + query_path(parent[nxt[u]], v);
    }

    void update_path(int u, int v, ll x) {
        if (tin[u] < tin[v]) swap(u, v);
        if (nxt[u] == nxt[v]) return seg.update(tin[v], tin[u], x);
        seg.update(tin[nxt[u]], tin[u], x); update_path(parent[nxt[u]], v, x);
    }

    ll query_subtree(int u) { return seg.query(tin[u], tin[u]+sz[u]-1); }
    void update_subtree(int u, ll x) { seg.update(tin[u], tin[u]+sz[u]-1, x); }

    int LCA(int u, int v) {
        if (tin[u] < tin[v]) swap(u, v);
        return nxt[u] == nxt[v] ? v : LCA(parent[nxt[u]], v);
    }
};