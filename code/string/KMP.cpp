#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// Knuth-Morris-Pratt (KMP + Optional DFA Construction)

// Exact pattern matching in O(N + M), including overlapping occurrences.
// Failure links preserve smaller matches when the current match fails.
// Optional DFA precomputes transitions for lowercase letters ('a' to 'z').

// 0-indexed pattern. States range from 0 to N.
// State i: longest pattern prefix matching a suffix of the processed text.
// nb[i]: longest proper border of the pattern prefix of length i.
// State N indicates a complete occurrence. Pattern must be non-empty.

// Time Complexity: O(N) to build failure links, O(N + M) to match.
// nxt: O(1) amortized during a sequential scan, O(N) worst case per call.
// DFA: O(26 * N) to build, O(1) per transition.
// Space Complexity: O(N) for failure links, O(26 * N) for the DFA.

// Call build_dfa() before querying dfa[state][c - 'a'].

struct KMP {
    string p;
    int n;
    vector<int> nb;
    vector<array<int, 26>> dfa;

    KMP(const string &p) : p(p), n((int)p.size()), nb(n+1), dfa(n+1) {
        for(int k = 1; k < n; k++)
            nb[k+1] = nxt(nb[k], p[k]);
    }

    int nxt(int i, char c) {
        for(; i; i = nb[i])
            if(i < n and p[i] == c)return i+1;
        return p[0] == c;
    }

    void build_dfa() {
        dfa[0][p[0]-'a'] = 1;

        for(int k = 1; k <= n; k++)
            for(int c = 0; c < 26; c++) {
                if(k < n and p[k] == 'a'+c)dfa[k][c] = k+1;
                else dfa[k][c] = dfa[nb[k]][c];
            }
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