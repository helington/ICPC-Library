#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// Euler's Totient Function (Phi Sieve + Factorization + Power Tower)

// Computes Euler's totient function using a sieve or prime factorization.
// Supports modular exponentiation of power towers using generalized Euler's theorem.

// Phi Sieve: O(N log log N) time, O(N) space.
// Single Phi Query: O(sqrt(N)) time, O(1) space.
// Power Tower: O(log M) recursive levels, O(log M) per level.
// Requires sieve() before power_tower(), with 1 <= M < MAXN.
// Power tower assumes positive bases.

const int MAXN = 1e6 + 5;
ll phi[MAXN];

void sieve() {
    for (int i = 0; i < MAXN; i++)
        phi[i] = i;
    for (int i = 2; i < MAXN; i++) if (phi[i] == i)
        for (int j = i; j < MAXN; j += i)
            phi[j] -= phi[j] / i;
}

ll get_phi(ll n) {
    ll result = n;
    for (ll i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0)
                n /= i;
            result -= result / i;
        }
    }
    if (n > 1)
        result -= result / n;
    return result;
}

ll qpow(ll b, ll e, ll m) {
    ll res = 1;
    for (; e; e /= 2) {
        if (e & 1) {
            res = (__int128) res * b;
            if (res >= m) res = res % m + m;
        }
        b = (__int128) b * b;
        if (b >= m) b = b % m + m;
    }
    return res;
}

ll power_tower(const vector<ll> &v, int idx, ll m) {
    if (m == 1) return 1;
    if (idx == v.size()) return 1;

    ll e = power_tower(v, idx + 1, phi[m]);
    return qpow(v[idx], e, m);
}