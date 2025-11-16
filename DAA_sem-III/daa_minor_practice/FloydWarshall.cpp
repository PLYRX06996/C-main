#include <bits/stdc++.h>
using namespace std;

void calcFloydWarshall(vector<vector<int>>& matrix) {
    int n = matrix.size();

    for (int i = 0; i < n; i++) {
        matrix[i][i] = 0;
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][k] != INT_MAX && matrix[k][j] != INT_MAX)
                    matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);
            }
        }
    }
}

int main() {
    int n, e;
    cout << "Enter number of vertices: ";
    cin >> n;
    cout << "Enter number of edges: ";
    cin >> e;

    vector<vector<int>> matrix(n, vector<int>(n, INT_MAX));

    cout << "\nEnter edges in format: source destination weight\n";
    cout << "(Vertices are numbered from 0 to " << n - 1 << ")\n";

    for (int i = 0; i < e; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        matrix[u][v] = w;
    }    
    calcFloydWarshall(matrix);

    cout << "\nAll-Pairs Shortest Path Matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == INT_MAX)
                cout << "INF\t";
            else
                cout << matrix[i][j] << "\t";
        }
        cout << "\n";
    }

    return 0;
}