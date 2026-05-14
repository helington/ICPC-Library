#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// Mo's Algorithm (Square Root Decomposition on Queries)

// Efficiently answers offline range queries by sorting them in a specific order
// to minimize the total movement of the left (L) and right (R) pointers.

// 0-indexed internally.
// Time Complexity: O((N + Q) * sqrt(N) * F), where F is the cost of add/remove.
// Space Complexity: O(N + Q)

const int BLOCK = 450; // Optimal value is usually around N / sqrt(Q)int q;
int q;

struct Query {
    int l, r, idx;
    bool operator < (Query &o) const {
        int b1 = l / BLOCK, b2 = o.l / BLOCK;
        if (b1 != b2) return b1 < b2;
        return (b1 & 1) ? (r < o.r) : (r > o.r);
    }
};

vector<ll> MO(vector<Query> &queries) {
    vector<ll> ans(q);
    sort(queries.begin(), queries.end());

    int L=0, R=-1;
    ll curr = 0;

    auto add = [&](int idx) {};
    auto remove = [&](int idx) {};

    for (auto [l, r, idx] : queries) {
        while (l < L) add(--L);
        while (r > R) add(++R);
        while (l > L) remove(L++);
        while (r < R) remove(R--);
        ans[idx] = curr;
    }

    return ans;
}