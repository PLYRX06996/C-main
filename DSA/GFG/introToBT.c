#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<limits.h>

typedef struct Node {
struct Node *left, *right;
int data;
} Node;
Node *createNode(int value) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}
void InOrderDFS(Node *root) {
    if(!root) return;
    InOrderDFS(root->left);
    printf("%d ", root->data);
    InOrderDFS(root->right); 
}
void PreOrderDFS(Node *root) {
    if(!root) return;
    printf("%d ", root->data);
    PreOrderDFS(root->left);
    PreOrderDFS(root->right); 
}
void PostOrder(Node *root) {
    if(!root) return;
    PostOrder(root->left);
    PostOrder(root->right);
    printf("%d ", root->data); 
}
void LevelOrderBFS(Node *root) {
    if(!root) return;
    Node *queue[100];
    int front = 0;
    int rear = 0;
    queue[rear++] = root;

    while (front < rear) {
        Node *temp = queue[front++];
        printf("%d ", temp->data);
        if(temp->left) queue[rear++] = root->left;
        if(temp->right) queue[rear++] = root->right;
    }
}
Node *BSTinsert(Node *root, int key) {
    if(!root) return createNode(key);
    if(root->data == key) return root;
    if(root->data < key) root->right = BSTinsert(root->right, key);
    else root->left = BSTinsert(root->left, key);
    return root;
}
Node *BSTsearch(Node *root, int key) {
    if(!root || root->data == key) return root;
    if(root->data < key) return BSTsearch(root->right, key);
    return BSTsearch(root->left, key);
}
Node *transplant(Node *t, Node *u, Node *root) { // t to be transplant with u
    if(root == t) return u;

    Node *parent = root;
    while(parent) {
        if(parent->left == t) {
            parent->left = u;
            break;
        } else if(parent->right == t) {
            parent->right = u;
            break;
        } else if(t->data < parent->data) parent = parent->left;
        else parent = parent->right;
    }
    return root;
}
Node *findMin(Node *root) {
    while(root && root->left) root = root->left;
    return root;
}
void BSTdelete(Node *t, Node **root) {
    if(!t) return;
    
    if(!t->left) *root = transplant(t , t->right, *root);
    else if(!t->right) *root = transplant(t, t->left, *root);

    else {
        Node *succ = findMin(t->right);
        if(succ != t->right) {
            *root = transplant(succ, succ->right, *root);
            succ->right = t->right;
        }
        succ->left = t->left;
        *root = transplant(t, succ, *root);
    }
    free(t);
}
void BSTDeletekey(Node *root, int key) {
    Node *t = BSTsearch(root, key);
    if(t) BSTdelete(t, root);
    else printf("Key not found in BST\n");
}
int Height(Node *root) { // this gives hight of BSt if we replace root with a node
    if(!root) return -1; // it will give hight of that node
    int LH = Height(root->left);
    int RH = Height(root->right);
    return (LH > RH ? LH : RH) + 1;
}
// Level(node) = depth(node) + 1
// depth(leaf) + height(leaf) = height(root)
int Depth(Node *root, Node *target) {
    int depth = 0;
    while(root) {
        if(root == target) return depth;
        else if(root->data < target->data) root = root->right;
        else root = root->left;
        depth++;
    }
    return -1;
}
void nodeDepth(Node *root, int key) {
    Node *node = BSTsearch(root, key);
    if(node) Depth(root, node);
}
void printLeafNodes(Node *root) {
    if(!root) return;
    if(!root->left && !root->right) {
        printf("%d ", root->data);
        return;
    }
    printLeafNodes(root->left);
    printLeafNodes(root->right);
}
int main() {
    
    return 0;
}