#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// Modular Arithmetic Structure (mod_int)

// Provides automated modular arithmetic operations.
// Supports addition, subtraction, multiplication, division, and exponentiation.

// 0-indexed values internally.
// Time Complexity: O(log MOD) for division and exponentiation, O(1) for others.
// Space Complexity: O(1)

template<ll MOD>
struct mod_int {
    ll v;

    mod_int(ll x = 0) : v((x % MOD + MOD) % MOD) {}

    mod_int operator + (const mod_int &b) const { ll a = v + b.v; return a < MOD ? a : a - MOD; }
    mod_int operator - (const mod_int &b) const { ll a = v - b.v; return a < 0 ? a + MOD : a; }
    mod_int operator * (const mod_int &b) const { return mod_int(v * b.v % MOD); }
    // IMPORTANTE: Sempre use parênteses devido à baixa precedência do ^ em C++. Ex: a * (b ^ c)
    mod_int operator ^ (ll e) const {
        mod_int res = 1;
        mod_int base = *this;
        while (e) {
            if (e & 1) res = res * base;
            base = base * base;
            e >>= 1;
        }
        return res;
    }
    mod_int operator / (const mod_int &b) const { return *this * (b ^ (MOD - 2)); }

    mod_int& operator += (const mod_int &b) { return *this = *this + b; }
    mod_int& operator -= (const mod_int &b) { return *this = *this - b; }
    mod_int& operator *= (const mod_int &b) { return *this = *this * b; }
    mod_int& operator /= (const mod_int &b) { return *this = *this / b; }
    mod_int& operator ^= (ll e) { return *this = *this ^ e; }

    bool operator == (const mod_int &b) const { return v == b.v; }
    bool operator != (const mod_int &b) const { return v != b.v; }
    bool operator <  (const mod_int &b) const { return v <  b.v; }

    friend ostream& operator << (ostream& os, const mod_int &o) { return os << o.v; }
    friend istream& operator >> (istream& is, mod_int &o) {
        ll x; is >> x;
        o = mod_int(x);
        return is;
    }
};

typedef mod_int<(ll)1e9 + 7> mint;