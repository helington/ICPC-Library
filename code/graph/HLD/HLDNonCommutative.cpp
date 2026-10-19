
#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// Heavy-Light Decomposition (Non-Commutative + Lazy Propagation)
// Supports associative, non-commutative path operations.
// Node/Tag pattern with forward and backward aggregates.
// Vertices are 1-indexed; segment tree positions are 0-indexed.
//
// Time Complexity:
// - Build: O(N)
// - Path Update: O(log^2 N)
// - Path Query: O(log^2 N)
// Space Complexity: O(N)

struct Tag {
    // TODO: update information
    bool has = false;

    void compose(const Tag &t) {
        // TODO: compose t after the current tag
        has = true;
    }
};

struct Node {
    // TODO: monoid information
    // Node() must be the identity element

    Node() {}

    void apply(const Tag &t, ll sz) {
        // TODO: apply t to this aggregate
    }
};

// a followed by b (order matters)
Node merge(const Node &a, const Node &b) {
    // TODO: associative operation
    return Node();
}

struct SegTree {
    struct Info {
        Node fw, bw;
    };

    int N;
    vector<Info> seg;
    vector<Tag> lazy;

    SegTree(int n) : N(n), seg(4 * n), lazy(4 * n) {}

    Info combine(const Info &a, const Info &b) {
        return {
            merge(a.fw, b.fw),
            merge(b.bw, a.bw)
        };
    }

    void apply(int p, int l, int r, const Tag &t) {
        seg[p].fw.apply(t, r - l + 1);
        seg[p].bw.apply(t, r - l + 1);
        lazy[p].compose(t);
    }

    void build(vector<Node> &v, int p, int l, int r) {
        if (l == r) {
            seg[p] = {v[l], v[l]};
            return;
        }

        int m = (l + r) / 2;

        build(v, 2 * p, l, m);
        build(v, 2 * p + 1, m + 1, r);

        seg[p] = combine(seg[2 * p], seg[2 * p + 1]);
    }

    void unlazy(int p, int l, int r) {
        if (!lazy[p].has || l == r) return;

        int m = (l + r) / 2;
        int e = 2 * p, d = e + 1;

        apply(e, l, m, lazy[p]);
        apply(d, m + 1, r, lazy[p]);

        lazy[p] = Tag();
    }

    Info query(int a, int b, int p, int l, int r) {
        if (b < l || r < a) return Info();
        if (a <= l && r <= b) return seg[p];

        unlazy(p, l, r);

        int m = (l + r) / 2;

        return combine(
            query(a, b, 2 * p, l, m),
            query(a, b, 2 * p + 1, m + 1, r)
        );
    }

    void update(int a, int b, int p, int l, int r, const Tag &t) {
        if (b < l || r < a) return;

        if (a <= l && r <= b) {
            apply(p, l, r, t);
            return;
        }

        unlazy(p, l, r);

        int m = (l + r) / 2;

        update(a, b, 2 * p, l, m, t);
        update(a, b, 2 * p + 1, m + 1, r, t);

        seg[p] = combine(seg[2 * p], seg[2 * p + 1]);
    }

    void build(vector<Node> &v) {
        build(v, 1, 0, N - 1);
    }

    Info query(int a, int b) {
        if (a > b) return Info();
        return query(a, b, 1, 0, N - 1);
    }

    void update(int a, int b, const Tag &t) {
        if (a > b) return;
        update(a, b, 1, 0, N - 1, t);
    }
};

template<bool EDGE = false>
struct HLD {
    int N, timer;
    vector<vector<int>> adj;
    vector<int> sz, dep, parent, tin, nxt;
    SegTree seg;

    HLD(int n) : N(n), timer(0), adj(n + 1),
        sz(n + 1), dep(n + 1), parent(n + 1),
        tin(n + 1), nxt(n + 1), seg(n) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int p) {
        parent[u] = p;
        sz[u] = 1;

        for (auto &v : adj[u]) if (v != p) {
            dep[v] = dep[u] + 1;
            dfs(v, u);
            sz[u] += sz[v];

            if (adj[u][0] == p || sz[v] > sz[adj[u][0]])
                swap(v, adj[u][0]);
        }
    }

    void hld(int u, int p) {
        tin[u] = timer++;

        for (auto v : adj[u]) if (v != p) {
            nxt[v] = (v == adj[u][0] ? nxt[u] : v);
            hld(v, u);
        }
    }

    void build(int root = 1) {
        timer = 0;
        dep[root] = 0;
        dfs(root, root);
        nxt[root] = root;
        hld(root, root);
    }

    // v[u] = vertex value
    // EDGE=true: v[u] = value of edge (parent[u],u)
    void build_values(vector<Node> &v) {
        vector<Node> a(N);

        for (int u = 1; u <= N; u++)
            a[tin[u]] = v[u];

        seg.build(a);
    }

    // Ordered query from u to v
    Node query(int u, int v) {
        Node left, right;

        while (nxt[u] != nxt[v]) {
            if (dep[nxt[u]] >= dep[nxt[v]]) {
                auto curr = seg.query(tin[nxt[u]], tin[u]);
                left = merge(left, curr.bw);
                u = parent[nxt[u]];
            } else {
                auto curr = seg.query(tin[nxt[v]], tin[v]);
                right = merge(curr.fw, right);
                v = parent[nxt[v]];
            }
        }

        if (dep[u] >= dep[v]) {
            auto curr = seg.query(tin[v] + EDGE, tin[u]);
            left = merge(left, curr.bw);
        } else {
            auto curr = seg.query(tin[u] + EDGE, tin[v]);
            right = merge(curr.fw, right);
        }

        return merge(left, right);
    }

    void update(int u, int v, const Tag &t) {
        while (nxt[u] != nxt[v]) {
            if (dep[nxt[u]] >= dep[nxt[v]]) {
                seg.update(tin[nxt[u]], tin[u], t);
                u = parent[nxt[u]];
            } else {
                seg.update(tin[nxt[v]], tin[v], t);
                v = parent[nxt[v]];
            }
        }

        if (dep[u] > dep[v]) swap(u, v);
        seg.update(tin[u] + EDGE, tin[v], t);
    }
};
