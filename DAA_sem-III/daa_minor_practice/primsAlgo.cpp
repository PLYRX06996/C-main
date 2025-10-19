#include <iostream>
#include <vector>
#include <climits>
#include <map>
using namespace std;

int minKey(vector<int>& key, vector<bool>& mstSet, int V) {
    int min = INT_MAX, minIndex = -1;
    for (int v = 0; v < V; v++) {
        if (!mstSet[v] && key[v] < min) {
            min = key[v];
            minIndex = v;
        }
    }
    return minIndex;
}

void printMST(vector<int>& parent, vector<vector<int>>& graph, int V) {
    cout << "\nMinimum Spanning Tree using Prim's Algorithm:\n";
    cout << "Edge \t\tWeight\n";
    cout << "----------------------\n";
    
    int totalWeight = 0;
    for (int i = 1; i < V; i++) {
        char u = 'A' + parent[i];
        char v = 'A' + i;
        cout << u << " - " << v << " \t\t" << graph[i][parent[i]] << endl;
        totalWeight += graph[i][parent[i]];
    }
    cout << "----------------------\n";
    cout << "Total MST Weight: " << totalWeight << endl;
}

void primMST(vector<vector<int>>& graph, int V) {
    vector<int> parent(V);
    vector<int> key(V, INT_MAX);
    vector<bool> mstSet(V, false);

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < V - 1; count++) {
        int u = minKey(key, mstSet, V);
        mstSet[u] = true;

        for (int v = 0; v < V; v++) {
            if (graph[u][v] && !mstSet[v] && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
            }
        }
    }
    printMST(parent, graph, V);
}

int main() {
    int V, E;
    
    cout << "Enter number of vertices: ";
    cin >> V;
    
    // Create adjacency matrix
    vector<vector<int>> graph(V, vector<int>(V, 0));
    
    cout << "Enter number of edges: ";
    cin >> E;
    
    cout << "\nEnter edges in format (u v w) where:\n";
    cout << "u and v are vertex letters (A=0, B=1, C=2, ...)\n";
    cout << "w is the weight\n\n";
    
    cout << "Example: For edge A-B with weight 7, enter: A B 7\n\n";
    
    for (int i = 0; i < E; i++) {
        char u_char, v_char;
        int weight;
        
        cout << "Enter edge " << i + 1 << ": ";
        cin >> u_char >> v_char >> weight;
        
        int u = u_char - 'A';
        int v = v_char - 'A';
        
        if (u < 0 || u >= V || v < 0 || v >= V) {
            cout << "Invalid vertex! Please use letters A to " << char('A' + V - 1) << endl;
            i--;
            continue;
        }
        
        graph[u][v] = weight;
        graph[v][u] = weight;
    }
    
    cout << "\nGraph Information:\n";
    cout << "Number of vertices: " << V << endl;
    cout << "Number of edges: " << E << endl;
    
    cout << "\nAdjacency Matrix:\n";
    cout << "  ";
    for (int i = 0; i < V; i++) {
        cout << char('A' + i) << " ";
    }
    cout << endl;
    
    for (int i = 0; i < V; i++) {
        cout << char('A' + i) << " ";
        for (int j = 0; j < V; j++) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
    
    primMST(graph, V);
    
    return 0;
}