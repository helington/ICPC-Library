
#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// Matrix Multiplication + Binary Exponentiation

// Supports modular matrix multiplication and fast matrix exponentiation.
// Useful for linear recurrences and matrix-based DP transitions.

// Multiplication: O(N^3) time.
// Exponentiation: O(N^3 log E) time.
// Space Complexity: O(N^2).
// Requires square matrices for exponentiation and dimensions <= MAXN.

const int MAXN = 2;
const ll MOD = 1e9 + 7;

ll mod_add(ll a, ll b) { return (a + b) % MOD; }
ll mod_mul(ll a, ll b) { return (a * b) % MOD; }

template<typename T>
struct Matrix {
    int n, m;
    T a[MAXN][MAXN];

    Matrix(int n = 0, int m = 0, bool ident = false) : n(n), m(m) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                a[i][j] = 0;

        if (ident) {
            for (int i = 0; i < n; i++)
                a[i][i] = 1;
        }
    }

    T* operator[](int i) {
        return a[i];
    }

    const T* operator[](int i) const {
        return a[i];
    }

    Matrix<T> operator*(const Matrix<T> &r) const {
        Matrix<T> res(n, r.m);

        for (int i = 0; i < n; i++) {
            for (int k = 0; k < m; k++) {
                if (a[i][k] == 0) continue;

                for (int j = 0; j < r.m; j++) {
                    res[i][j] = mod_add(
                        res[i][j],
                        mod_mul(a[i][k], r[k][j])
                    );
                }
            }
        }

        return res;
    }

    Matrix<T> fexp(ll e) const {
        Matrix<T> res(n, n, true);
        Matrix<T> base = *this;

        while (e) {
            if (e & 1LL) res = res * base;
            base = base * base;
            e >>= 1LL;
        }

        return res;
    }
};
