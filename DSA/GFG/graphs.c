#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<limits.h>
#include<string.h>

#define MAX 100
typedef struct Node{
    int vertex;
    //int weight; --> for weighted graph
    struct Node *next;
}Node;
Node *adjList[MAX];
int visited[MAX];

void addEdge(int u, int v) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->next = adjList[u];
    adjList[u] = newNode;

    //for an undirected graph
    // newNode->vertex = u;
    // newNode->next = adjList[v];
    // adjList[v] = newNode;
}
void BFS(int startNode) {
    int queue[MAX], front = 0, rear = 0;

    visited[startNode] = 1;
    queue[rear++] = startNode;

    while(front < rear) {
        int current = queue[front++];
        printf("%d ", current);

        Node *temp = adjList[current];

        while(temp) {
            int v = temp->vertex;
            if(!visited[v]) {
                visited[v] = 1;
                queue[rear++] = v;
            }
            temp = temp->next;
        }
    }
}
void DFS(int startNode) {
    visited[startNode] = 1;
    printf("%d ", startNode);

    Node *temp = adjList[startNode];
    while(temp) {
        int neighbour = temp->vertex;
        if(!visited[neighbour]) {
            DFS(neighbour);
        }
        temp = temp->next;
    }
}
int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    // int adjmatrix[n][m];
    // for(int i =0; i < m; i++) {
    //     int u, v;
    //     scanf("%d %d", &u, &v);
    //     adjmatrix[u][v] = 1;
    //     adjmatrix[v][u] = 1;
    // }
    return 0;
}