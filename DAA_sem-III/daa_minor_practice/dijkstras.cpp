#include<bits/stdc++.h>
using namespace std;

#define INF numeric_limits<int>::max()

// Dijkstra's Algorithm
vector<int> dijkstra(int V, int start, vector<vector<pair<int, int>>>& graph) {
    vector<int> dist(V, INF);
    dist[start] = 0;           

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto edge : graph[u]) {
            int v = edge.first;
            int w = edge.second;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

void displayDistances(const vector<int>& dist, int start) {
    cout << "Vertex\tDistance from Source (" << start << ")\n";
    for (size_t i = 0; i < dist.size(); ++i) {
        if (dist[i] == INF)
            cout << i << "\tINF\n";
        else
            cout << i << "\t" << dist[i] << "\n";
    }
}

int main() {
    int V, E;
    cout << "Enter number of vertices: ";
    cin >> V;
    cout << "Enter number of edges: ";
    cin >> E;

    vector<vector<pair<int, int>>> graph(V);

    cout << "Enter edges in format (u v w):\n";
    for (int i = 0; i < E; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }

    int start;
    cout << "Enter starting vertex: ";
    cin >> start;
    displayDistances(dijkstra(V, start, graph), start);
    return 0;
}