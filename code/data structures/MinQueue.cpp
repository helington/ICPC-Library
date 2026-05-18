#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// MinQueue (Monotonic Queue)

// A queue data structure that supports finding the minimum element in O(1).
// Useful for sliding window problems where you need the minimum of a subarray.

// 0-indexed internally.
// Time Complexity: O(1) amortized per operation.
// Space Complexity: O(N) where N is the maximum number of elements.

template<typename T>
struct MinQueue {
    deque<pair<T, int>> dq;
    int sz = 0;

    T min() { return dq.front().first; }

    void push(T x) {
        int cnt = 1;
        while (!dq.empty() && x <= dq.back().first) {
            cnt += dq.back().second;
            dq.pop_back();
        }
        dq.push_back({x, cnt});
        sz++;
    }

    void pop() {
        if (dq.front().second > 1) dq.front().second--;
        else dq.pop_front();
        sz--;
    }

    bool empty() { return sz == 0; }
    int size() { return sz; }
};