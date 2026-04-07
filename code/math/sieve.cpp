#include <bits/stdc++.h>

using namespace std;

// Sieve of Eratosthenes

// Generates all prime numbers up to MAX_N.

// O(n log(log(n)))

const int MAXN = 1000005; // Adjust based on problem constraints (usually 1e6 or 1e7)

vector<int> sieve() {
    vector<int> primes;
    vector<bool> is_prime(MAXN, false);
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i <= MAXN; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            for (int j = 2 * i; j <= MAXN; j += i)
                is_prime[j] = false;
        }
    }

    return primes;
}

// Sieve to get the SPF of the numbers

vector<int> sieve() {
    vector<int> spf(MAXN, 0);
    spf[0] = spf[1] = 0;

    for (int i = 2; i <= MAXN; i++) {
        if (!spf[i]) {
            spf[i] = i;
            for (int j = 2 * i; j <= MAXN; j += i)
                if (!spf[j])
                    spf[j] = i;
        }
    }

    return spf;
}