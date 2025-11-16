#include <iostream>
#include <vector>
#include <climits>
using namespace std;

struct Edge {
    int u, v, w;
};

void bellmanFord(int n, int e, vector<Edge>& edges, int source) {
    vector<int> dist(n, INT_MAX);
    dist[source] = 0;

    for (int i = 1; i <= n - 1; i++) {
        for (int j = 0; j < e; j++) {
            int u = edges[j].u;
            int v = edges[j].v;
            int w = edges[j].w;

            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }

    bool negCycle = false;
    for (int j = 0; j < e; j++) {
        int u = edges[j].u;
        int v = edges[j].v;
        int w = edges[j].w;

        if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
            negCycle = true;
            break;
        }
    }

    if (negCycle) {
        cout << "\nGraph contains a negative-weight cycle!\n";
    } else {
        cout << "\nVertex\tDistance from Source " << source << "\n";
        for (int i = 0; i < n; i++) {
            if (dist[i] == INT_MAX)
                cout << i << "\t" << "INF" << "\n";
            else
                cout << i << "\t" << dist[i] << "\n";
        }
    }
}

int main() {
    int n, e;
    cout << "Enter number of vertices: ";
    cin >> n;
    cout << "Enter number of edges: ";
    cin >> e;

    vector<Edge> edges(e);

    cout << "\nEnter edges in format: source destination weight\n";
    cout << "(Vertices are numbered from 0 to " << n - 1 << ")\n";
    for (int i = 0; i < e; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    int source;
    cout << "\nEnter the source vertex: ";
    cin >> source;

    bellmanFord(n, e, edges, source);

    return 0;
}