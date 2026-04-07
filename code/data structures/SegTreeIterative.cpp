#include <bits/stdc++.h>

using namespace std;

// Iterative Segment Tree (Point Update, Range Query)

// 0-indexed. 
// Queries are in the range [l, r) -> 'l' inclusive, 'r' exclusive.

// Time Complexity: 
//   - Build: O(N)
//   - Update: O(log N)
//   - Query: O(log N)
// Space Complexity: O(2 * N)

template <typename T>
struct SegTree {
    int N;
    vector<T> seg;

    const T NEUTRAL = 0;

    T join(const T &vl, const T &vr) { return vl + vr; }

    SegTree(int n) : N(n), seg(2 * n, NEUTRAL) {}

    SegTree(vector<T> &v) : N(v.size()), seg(2 * v.size(), NEUTRAL) {
        for (int i = 0; i < N; i++)
            seg[i + N] = v[i];
        for (int i = N - 1; i > 0; i--)
            seg[i] = join(seg[i << 1], seg[i << 1 | 1]);
    }

    T query (int l, int r) {
        T ans_l = NEUTRAL, ans_r = NEUTRAL;
        for (l += N, r += N; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ans_l = join(ans_l, seg[l++]);
            if (r & 1) ans_r = join(seg[--r], ans_r);
        }
        return join(ans_l, ans_r);
    }

    void update(int p, T val) {
        for (seg[p += N] = val; p >>= 1;)
            seg[p] = join(seg[p << 1], seg[p << 1 | 1]);
    }
};