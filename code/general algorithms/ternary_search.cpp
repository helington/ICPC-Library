#include <bits/stdc++.h>

using namespace std;
#define ll long long

// Ternary Search (Continuous / Double)

// Finds the extremum (min/max) of a strictly unimodal function.

// Time Complexity: O(ITERATIONS * T_check)

const int INTERATIONS = 60; // Decrease if it's getting TLE, increase if it's getting WA

double check(double m) {
    // Write some Unimodal function (Quadract funtion with max or min)
}

double ternary_search(double l, double r) {
    for (int i = 0; i < INTERATIONS; i++) {
        double m1 = l + (r - l) / 3;
        double m2 = r - (r - l) / 3;

        // CURRENTLY FINDING MINIMUM
        // If finding MAXIMUM, change to: "<" to ">"
        if (check(m1) < check(m2))
            l = m1;
        else
            r = m2;
    }

    return (l + r) / 2;
}

// Ternary Search (Discrete / Integer)
//
// Safest implementation to avoid infinite loops with integer division.

ll check(ll x) {
    // Write some Unimodal function (Quadract funtion with max or min)
}

ll ternary_search_int(ll l, ll r) {
    while (r - l > 2) {
        ll m1 = l + (r - l) / 3;
        ll m2 = r - (r - l) / 3;

        if (check(m1) < check(m2))
            r = m2;
        else
            l = m1;
    }

    // The range is now very small (at most 3 elements).
    // We just iterate through them to find the exact answer.
    ll ans = l;
    for (long long i = l + 1; i <= r; i++)
        if (check(i) < check(ans))
            ans = i;

    return ans;
}