#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'

// Dijkstra (Heap version)

// Computes the lowest cost from vertex 'b' to all other vertices.
// If dist[i] == INF, vertex 'i' is UNREACHABLE from 'b'.

// O((n + m)log(m))

// For all pairs: O(n³log(n))

const int MAXN = 1e5 + 5;
const ll INF = 1e18;
vector<pair<ll, int>> adj[MAXN];
int n;

vector<ll> dijkstra(int start=1) {
    vector<ll> dist(MAXN, INF);
    priority_queue<
        pair<ll, int>,
        vector<pair<ll, int>>,
        greater<pair<ll, int>>
    > pq;

    dist[start] = 0;
    pq.push({0, start});

    while(!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();

        if (d > dist[u]) continue;

        for(auto &[v, w] : adj[u]) {
            if (dist[v] > d + w) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}

// Dijkstra (Non-Heap version)

// Computes the lowest cost from vertex 'b' to all other vertices.
// If dist[i] == LLONG_MAX, vertex 'i' is UNREACHABLE from 'b'.

// O(n²)

// For all pairs: O(n³)

vector<ll> dijkstra(int start=1) {
    vector<ll> dist(MAXN, INF);
    dist[start] = 0;

    vector<bool> visited(n, false);

    for (int i = 0; i < n; i++) {
        int u = -1;
        ll mn = INF;

        for (int j = 0; j < n; j++) {
            if ((!visited[j]) and (dist[j] < mn)) {
                mn = dist[j];
                u = j;
            }
        }
        if (u==-1) break;

        visited[u] = true;

        for (auto [v, w] : adj[u])
            if (dist[v] > dist[u] + w)
                dist[v] = dist[u] + w;
    }

    return dist;
}