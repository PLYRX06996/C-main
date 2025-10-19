#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Edge {
    int src, dest, weight;
    
    Edge(int s, int d, int w) : src(s), dest(d), weight(w) {}
    
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class DSU {
private:
    vector<int> parent, rank;
    
public:
    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX == rootY) return;
        
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
};

vector<Edge> kruskalMST(int vertices, vector<Edge>& edges) {
    sort(edges.begin(), edges.end());
    
    DSU dsu(vertices);
    
    vector<Edge> mst;
    int mstWeight = 0;
    int edgesUsed = 0;
    
    for (const Edge& edge : edges) {
        if (dsu.find(edge.src) != dsu.find(edge.dest)) {
            mst.push_back(edge);
            mstWeight += edge.weight;
            dsu.unite(edge.src, edge.dest);
            edgesUsed++;
            
            if (edgesUsed == vertices - 1) {
                break;
            }
        }
    }
    
    cout << "Total MST Weight: " << mstWeight << endl;
    return mst;
}

void displayMST(const vector<Edge>& mst) {
    cout << "\nMinimum Spanning Tree using Kruskal's Algorithm:\n";
    cout << "Source -> Destination (Weight)\n";
    cout << "--------------------------------\n";
    
    for (const Edge& edge : mst) {
        char src = 'A' + edge.src;
        char dest = 'A' + edge.dest;
        cout << "   " << src << "   ->   " << dest << "   ( " << edge.weight << " )\n";
    }
}

int main() {    
    int V, E;
    
    cout << "Enter number of vertices: ";
    cin >> V;
    cout << "Enter number of edges: ";
    cin >> E;
    
    vector<Edge> edges;
    
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
        
        edges.push_back(Edge(u, v, weight));
    }
    
    cout << "\nGraph Information:\n";
    cout << "Number of vertices: " << V << endl;
    cout << "Number of edges: " << E << endl;
    
    cout << "\nAll Edges:\n";
    for (const Edge& edge : edges) {
        char src = 'A' + edge.src;
        char dest = 'A' + edge.dest;
        cout << src << " - " << dest << " : " << edge.weight << endl;
    }
    
    vector<Edge> mst = kruskalMST(V, edges);
    displayMST(mst);
    
    return 0;
}