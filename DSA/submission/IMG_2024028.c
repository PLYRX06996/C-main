#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef struct Node_t {
    int data;
    struct Node_t *left;
    struct Node_t *right;
} Node_t;

Node_t *createNode(int data) {
    Node_t *newNode = (Node_t *)malloc(sizeof(Node_t));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node_t *insert(Node_t *root, int data) {
    if(root == NULL) {
        return createNode(data);
    }
    if(data < root->data) {
        root->left = insert(root->left, data);
    } else if(data > root->data) {
        root->right = insert(root->right, data);
    }
    return root;
}

void preorder(Node_t *root) {
    if(root == NULL) {
        return;
    }
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

void inorder(Node_t *root) {
    if(root == NULL) {
        return;
    }
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

void postorder(Node_t *root) {
    if(root == NULL) {
        return;
    }
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

int maxDepth(Node_t *root) {
    if (root == NULL) {
        return 0;
    }
    int leftDepth = maxDepth(root->left);
    int rightDepth = maxDepth(root->right);
    return (leftDepth > rightDepth) ? leftDepth + 1 : rightDepth + 1;
}

void printNodesAtDepth(Node_t *root, int currentDepth, int targetDepth) {
    if (root == NULL) {
        return;
    }
    if (currentDepth == targetDepth) {
        if (root->left == NULL && root->right == NULL) {
            printf("%d ", root->data);
        }
        return;
    }
    printNodesAtDepth(root->left, currentDepth + 1, targetDepth);
    printNodesAtDepth(root->right, currentDepth + 1, targetDepth);
}

void printDeepestLeafNodes(Node_t *root) {
    if (root == NULL) {
        return;
    }
    int depth = maxDepth(root);
    printNodesAtDepth(root, 1, depth);
}

int main() {
    int T;
    scanf("%d", &T);
    while(T--){
        int n;
        scanf("%d", &n);
        int arr[n];
        for(int i = 0; i < n; i++){
            scanf("%d", &arr[i]);
        }
        Node_t *root = NULL;
        for(int j = 0; j < n; j++) {
            root = insert(root, arr[j]);
        }
        preorder(root);
        printf("\n");
        inorder(root);
        printf("\n");
        postorder(root);
        printf("\n");
        printDeepestLeafNodes(root);
        printf("\n");
    }
    return 0;
}