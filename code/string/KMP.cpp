#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// Knuth-Morris-Pratt (KMP)

// Exact pattern matching in O(N + M), including overlapping occurrences.
// Failure links preserve smaller matches when the current match fails.

// 0-indexed pattern. States range from 0 to M.
// State i: longest prefix of the pattern matching a suffix of the processed text.
// nb[i]: longest proper border of the pattern prefix of length i.
// State M indicates a complete occurrence. Pattern must be non-empty.

// Time Complexity: O(M) to build failure links, O(N + M) to match.
// nxt: O(1) amortized during a sequential scan,
//       although a single call can take O(M) in the worst case.
// Space Complexity: O(M).

struct KMP {
    string p;
    int n;
    vector<int> nb;

    KMP(const string &p) : p(p), n((int)p.size()), nb(n+1) {
        for(int k = 1; k < n; k++)
            nb[k+1] = nxt(nb[k], p[k]);
    }

    int nxt(int i, char c) {
        for(; i; i = nb[i])
            if(i < n and p[i] == c)return i+1;
        return p[0] == c;
    }
};

int matches(const string &p, const string &s) {
    KMP kmp(p);
    int cnt = 0, state = 0;

    for(char c : s) {
        state = kmp.nxt(state, c);
        if(state == kmp.n)cnt++;
    }

    return cnt;
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    string s, p; cin >> s >> p;
    cout << matches(p, s) << endl;

    return 0;
}