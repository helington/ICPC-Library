#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// Knuth-Morris-Pratt (KMP Automaton)

// Deterministic finite automaton for exact pattern matching.
// Each state represents the length of the longest prefix of the pattern
// matching a suffix of the text processed so far.
// State n means that a complete occurrence of the pattern was found.

// Alphabet: lowercase English letters ('a' to 'z').
// States range from 0 to N, where N = pattern length.
// dfa[state][c] gives the next state after reading character c.

// Overlapping occurrences are preserved automatically through fallback transitions.

// Time Complexity: O(26 * N) to build.
// Query Complexity: O(1) per character.
// Space Complexity: O(26 * N).

vector<array<int, 26>> build_dfa(const string &p) {
    int n = p.length();
    vector<array<int, 26>> dfa(n + 1);
    if (n == 0) return dfa;
    dfa[0][p[0] - 'a'] = 1;
    for (int i = 1, j = 0; i <= n; i++) {
        for (int c = 0; c < 26; c++)
            dfa[i][c] = dfa[j][c];
        if (i < n) {
            dfa[i][p[i] - 'a'] = i + 1;
            j = dfa[j][p[i] - 'a'];
        }
    }
    return dfa;
}