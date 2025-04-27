#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Node_t {
    int data;
    struct Node_t *left;
    struct Node_t *right;
} Node_t;

typedef struct {
    int parent, child;
} Pair;

Node_t* createNode(int data) {
    Node_t* newNode = (Node_t*)malloc(sizeof(Node_t));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node_t* insert(Node_t* root, int data) {
    if (!root) return createNode(data);
    if (data < root->data) root->left = insert(root->left, data);
    else if (data > root->data) root->right = insert(root->right, data);
    return root;
}

void preorder(Node_t* root) {
    if (!root) return;
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

void inorder(Node_t* root) {
    if (!root) return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

void postorder(Node_t* root) {
    if (!root) return;
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

void collectPairs(Node_t* root, int d, Pair *out, int *count) {
    if (!root) return;
    if (root->left && abs(root->data - root->left->data) <= d) {
        out[*count].parent = root->data;
        out[*count].child = root->left->data;
        (*count)++;
    }
    collectPairs(root->left, d, out, count);
    collectPairs(root->right, d, out, count);
    if (root->right && abs(root->data - root->right->data) <= d) {
        out[*count].parent = root->data;
        out[*count].child = root->right->data;
        (*count)++;        
    }
}

int comparePair(const void *a, const void *b) {
    const Pair *A = (const Pair *)a;
    const Pair *B = (const Pair *)b;
    if (A->parent != B->parent) return A->parent - B->parent;
    return A->child - B->child;
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        int n, d;
        scanf("%d %d", &n, &d);
        Node_t* root = NULL;
        for (int i = 0; i < n; i++) {
            int val;
            scanf("%d", &val);
            root = insert(root, val);
        }

        preorder(root); 
        printf("\n");
        inorder(root); 
        printf("\n");
        postorder(root); 
        printf("\n");

        Pair *temp = (Pair*)malloc(n * sizeof(Pair));
        int count = 0;
        collectPairs(root, d, temp, &count);
        qsort(temp, count, sizeof(Pair), comparePair);

        for (int i = 0; i < count; i++) {
            printf("%d %d\n", temp[i].parent, temp[i].child);
        }

        free(temp);
    }
    return 0;
}
