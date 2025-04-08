#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<limits.h>
#include<math.h>
#include<stdlib.h>

typedef struct treeNode {
    int data;
    struct treeNode *left;
    struct treeNode *right;
} treeNode;
treeNode *createNode(int value) {
    treeNode *newNode = (treeNode*)malloc(sizeof(treeNode));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}
void inOrderTravel(treeNode *root) {
    if(root == NULL) {
        return;
    }
    inOrderTravel(root->left);
    printf("%d ", root->data);
    inOrderTravel(root->right);
}
void preOrderTravel(treeNode *root) {
    if(root == NULL) {
        return;
    }
    printf("%d ", root->data);
    preOrderTravel(root->left);
    preOrderTravel(root->right);
}
void postOrderTravel(treeNode *root) {
    if(root == NULL) {
        return;
    }
    postOrderTravel(root->left);
    postOrderTravel(root->right);
    printf("%d ", root->data);
}
void levelOrderTravel(treeNode *root, int size) {
    if(root == NULL) {
        return;
    }
    treeNode **queue = (treeNode**)malloc(size * sizeof(treeNode*));
    int front = 0, rear = 0;
    queue[rear++] = root;
    
    while(front < rear) {
        treeNode *node = queue[front++];
        printf("%d ", node->data);
        
        if(node->left) {
            queue[rear++] = node->left;
        }
        if(node->right) {
            queue[rear++] = node->right;
        }
    }
    free(queue);
}
treeNode* insertBST(treeNode* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }
    if (value < root->data) {
        root->left = insertBST(root->left, value);
    } else {
        root->right = insertBST(root->right, value);
    }
    return root;
}
treeNode* buildBST(int arr[], int size) {
    treeNode* root = NULL;
    for(int i = 0; i < size; i++) {
        root = insertBST(root, arr[i]);
    }
    return root;
}
int main(){
    int tc;
    scanf("%d", &tc);
    while(tc--) {
        int n;
        scanf("%d", &n);
        int arr[n];
        for(int i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }
        treeNode *root = buildBST(arr, n);        
        preOrderTravel(root);
        printf("\n");
        inOrderTravel(root);
        printf("\n");
        postOrderTravel(root);
        printf("\n");
        levelOrderTravel(root, n);
        printf("\n");
    }
    return 0;
}