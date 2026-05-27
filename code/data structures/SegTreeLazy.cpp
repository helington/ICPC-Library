#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// Segment Tree with Lazy Propagation (Node/Tag Pattern)

// Advanced and highly versatile Segment Tree implementation.
// Uses separate Node and Tag (Lazy) structures to cleanly decouple
// tree logic from specific range operations (Sum, Min, Max, etc.).

// 0-indexed internally (ranges). Tree nodes are 1-indexed.
// Time Complexity: 
// - Build: O(N)
// - Range Update: O(log N)
// - Range Query: O(log N)
// Space Complexity: O(4 * N)

struct Tag {
    ll op;

    Tag() : op(0) {}
    Tag(ll v) : op(v) {}

    void compose(Tag &t) { op += t.op; }
};

struct Node {
    ll val;

    Node() : val(0) {}
    Node(ll v) : val(v) {}

    void apply(Tag &t, ll sz) { val += t.op * sz; }
};

Node merge(Node n1, Node n2) { return Node(n1.val + n2.val); }

struct SegTree {
    int N;
    vector<Node> seg;
    vector<Tag> lazy;

    SegTree(int n) : N(n), seg(4 * n), lazy(4 * n) {}

    SegTree(vector<ll> &v) : N(v.size()), seg(4 * N), lazy(4 * N) {
        build(v, 1, 0, N - 1);
    }

    void build(vector<ll> &v, int p, int l, int r) {
        if (l == r) {
            seg[p] = v[l];
            return;
        }

        int m = (l + r) / 2;

        build(v, 2 * p, l, m);
        build(v, 2 * p + 1, m + 1, r);
        
        seg[p] = merge(seg[2 * p], seg[2 * p + 1]);
    }

    void unlazy(int p, int l, int r) {
        if (l == r) return;

        int m = (l + r) / 2;
        int e = 2 * p, d = e + 1;

        seg[e].apply(lazy[p], m - l + 1);
        seg[d].apply(lazy[p], r - m);

        lazy[e].compose(lazy[p]);
        lazy[d].compose(lazy[p]);

        lazy[p] = Tag();
    }

    Node query(int a, int b, int p, int l, int r) {
        if (b < l or r < a) return Node();
        if (a <= l and r <= b) return seg[p];

        unlazy(p, l, r);

        int m = (l + r) / 2;

        return merge(query(a, b, 2 * p, l, m), 
                    query(a, b, 2 * p + 1, m + 1, r));
    }

    void update(int a, int b, int p, int l, int r, Tag &t) {
        if (b < l or r < a) return;
        if (a <= l and r <= b) {
            ll sz = r - l + 1;
            seg[p].apply(t, sz);
            lazy[p].compose(t);
            return;
        }

        unlazy(p, l, r);
        
        int m = (l + r) / 2;
        
        update(a, b, 2 * p, l, m, t);
        update(a, b, 2 * p + 1, m + 1, r, t);
        
        seg[p] = merge(seg[2 * p], seg[2 * p + 1]);
    }
    
    ll query(int a, int b) { return query(a, b, 1, 0, N - 1).val; }
    void update(int a, int b, ll val) { Tag t(val); update(a, b, 1, 0, N - 1, t); }
};