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
int depth(treeNode* root) {
    if (root == NULL)
        return 0;
    int LeftDepth = depth(root->left);
    int RightDepth = depth(root->right);

    return (LeftDepth > RightDepth ? LeftDepth : RightDepth) + 1;
}
void search(treeNode* root, int key) {
    treeNode* current = root;
    treeNode* parent = NULL;
    int depth = 1;
    int pathSum = 0;

    while (current != NULL) {
        pathSum += current->data;
        if (current->data == key) {
            if (parent == NULL) {
                printf("%d %d\n", depth, pathSum);
            } else {
                treeNode* sibling = (parent->left == current) ? parent->right : parent->left;
                if (sibling)
                    printf("%d %d %d\n", depth, pathSum, sibling->data);
                else
                    printf("%d %d\n", depth, pathSum);
            }
            return;
        }
        parent = current;
        if (key < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
        depth++;
    }
    printf("-1\n");
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
        int q;
        scanf("%d", &q);
        int queries[q];
        for(int i = 0; i < q; i++) {
            scanf("%d", &queries[i]);
        }
        printf("%d\n", depth(root));
        for(int i = 0; i < q; i++) {
            search(root, queries[i]);
        }
    }
    return 0;
}