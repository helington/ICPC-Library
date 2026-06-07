#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// DSU with Rollback (Disjoint Set Union)

// A Disjoint Set Union data structure that supports undoing the last operations.
// IMPORTANT: Path compression is INTENTIONALLY OMITTED to maintain O(log N) 
// strict depth and allow fast O(1) rollbacks.

// 1-indexed internally by default (0-indexed works seamlessly).
// Time Complexity: 
// - Find: O(log N)
// - Unite: O(log N)
// - Rollback: O(1) per undone operation
// Space Complexity: O(N + Q) where Q is the number of un-rolled back unions

struct DSU {
    vector<int> id, sz, savept;
    stack<pair<int &, int>> st;

    DSU(int n) : id(n+1), sz(n+1) { iota(id.begin(), id.end(), 0); }

    int find(int u) { return u == id[u] ? u : find(id[u]); }

    void unite(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) return;
        if (sz[u] < sz[v]) swap(u, v);
        save(id[v]); id[v] = u;
        save(sz[u]); sz[u] += sz[v];
    }

    void save(int &x) { st.emplace(x, x); }

    void pop() {
        st.top().first = st.top().second; st.pop();
        st.top().first = st.top().second; st.pop();
    }

    void checkpoint() { savept.push_back(st.size()); }

    void rollback() {
        while (st.size() > savept.back()) pop();
        savept.pop_back();
    }
};