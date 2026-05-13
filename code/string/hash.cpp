#include <bits/stdc++.h>
#include <random>
using namespace std;
#define ll long long
#define endl '\n'

// String Hashing (Double Hash + RNG Base)

// Fast substring comparisons in O(1).
// Highly resistant to collision attacks due to double hashing and randomized base.

// 0-indexed internally (ranges).
// Time Complexity: O(N) to build, O(1) per query.
// Space Complexity: O(N)

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

int uniform(int l, int r) {
    uniform_int_distribution<int> uid(l, r);
    return uid(rng);
}

template<ll MOD1 = 1000000007, ll MOD2 = 1000000009>
struct Hasher {
    int n;
    static ll BASE;
    vector<ll> hash1, power1, hash2, power2;

    Hasher() : n(0) {}

    Hasher(string &s) : n(s.size()), hash1(n), power1(n), hash2(n), power2(n) {
        if (n == 0) return;
        
        power1[0] = power2[0] = 1;
        hash1[0] = hash2[0] = s[0];
        
        for (int i = 1; i < n; i++) {
            power1[i] = (BASE * power1[i - 1]) % MOD1;
            power2[i] = (BASE * power2[i - 1]) % MOD2;
            
            hash1[i] = (hash1[i - 1] * BASE + (ll) s[i]) % MOD1;
            hash2[i] = (hash2[i - 1] * BASE + (ll) s[i]) % MOD2;
        }
    }

    ll operator() (int l, int r) const {
        if (!l) return (hash1[r] << 30) ^ hash2[r];

        ll ans1 = ((hash1[r] - (hash1[l-1] * power1[r - l + 1])) % MOD1 + MOD1) % MOD1;
        ll ans2 = ((hash2[r] - (hash2[l-1] * power2[r - l + 1])) % MOD2 + MOD2) % MOD2;

        // Pack the two 30-bit numbers into a single 64-bit integer
        return (ans1 << 30) ^ ans2;
    }
};

template<ll MOD1, ll MOD2> 
ll Hasher<MOD1, MOD2>::BASE = uniform(256, MOD1 - 1);