
#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// Mobius Function (Linear Sieve)

// Computes the Mobius function for all integers up to MAXN.
// mu(n) = 0 if n has a squared prime factor.
// Otherwise, mu(n) = (-1)^k, where k is the number of distinct prime factors.

// Time Complexity: O(N).
// Space Complexity: O(N).

const int MAXN = 1e5 + 5;
int mu[MAXN], comp[MAXN];
vector<int> primes;

void mobius() {
    mu[1] = 1;
    for (int i = 2; i < MAXN; i++) {
        if (!comp[i]) primes.push_back(i), mu[i] = -1;
        for (int p : primes) {
            if (p > (MAXN - 1) / i) break;
            comp[i * p] = 1;
            if (i % p == 0) { mu[i * p] = 0; break; }
            mu[i * p] = -mu[i];
        }
    }
}
