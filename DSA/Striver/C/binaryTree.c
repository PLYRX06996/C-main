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
    printf("\n");
}
void preOrderTravel(treeNode *root) {
    if(root == NULL) {
        return;
    }
    printf("%d ", root->data);
    inOrderTravel(root->left);
    inOrderTravel(root->right);
    printf("\n");
}
void postOrderTravel(treeNode *root) {
    if(root == NULL) {
        return;
    }
    inOrderTravel(root->left);
    inOrderTravel(root->right);
    printf("%d ", root->data);
    printf("\n");
}
void levelOrderTravel(treeNode *root) {
    if(root == NULL) {
        return;
    }
    treeNode *queue[] = (treeNode*)malloc(sizeof(treeNode));
    int front = 0 , rear = 0;
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
    printf("\n");
}
treeNode *buildFromPreOrder(int arr[], int size) {
    if(size == 0) return NULL;

    treeNode *root = createNode(arr[0]);
    int leftSize = (size - 1)/2;
    int rightSize = size - 1 - leftSize;

    if(leftSize > 0) {
        root->left = buildFromPreOrder(arr + 1, leftSize);
    }
    if(rightSize > 0) {
        root->right = buildFromPreOrder(arr + 1 + leftSize, rightSize);
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
        treeNode *root = NULL;
        root = buildFromPreOrder(arr, n);
        preOrderTravel(root);
        inOrderTravel(root);
        postOrderTravel(root);
        levelOrderTravel(root);
    }

    return 0;
}