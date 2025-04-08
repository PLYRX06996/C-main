#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *left, *right, *parent;
} Node;

Node *createNode(int data, Node *parent)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    newNode->parent = parent;
    return newNode;
}

Node *insertBST(Node *root, int data)
{
    if (root == NULL)
        return createNode(data, NULL);

    Node *current = root;
    Node *parent = NULL;

    while (current != NULL)
    {
        parent = current;
        if (data < current->data)
        {
            current = current->left;
        }
        else if (data > current->data)
        {
            current = current->right;
        }
        else
        {
            return root;
        }
    }

    if (data < parent->data)
    {
        parent->left = createNode(data, parent);
    }
    else
    {
        parent->right = createNode(data, parent);
    }

    return root;
}

Node *search(Node *root, int key)
{
    while (root != NULL && root->data != key)
    {
        if (key < root->data)
            root = root->left;
        else
            root = root->right;
    }
    return root;
}

Node *minimum(Node *node)
{
    while (node && node->left)
        node = node->left;
    return node;
}

Node *transplant(Node *root, Node *u, Node *v)
{
    if (u->parent == NULL)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v != NULL)
        v->parent = u->parent;
    return root;
}

Node *deleteNode(Node *root, Node *z)
{
    if (z->left == NULL)
    {
        root = transplant(root, z, z->right);
    }
    else if (z->right == NULL)
    {
        root = transplant(root, z, z->left);
    }
    else
    {
        Node *y = minimum(z->right);
        if (y->parent != z)
        {
            root = transplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        root = transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
    }
    free(z);
    return root;
}

void preOrderTravel(Node *root)
{
    if (root == NULL)
        return;
    printf("%d ", root->data);
    preOrderTravel(root->left);
    preOrderTravel(root->right);
}

void inOrderTravel(Node *root)
{
    if (root == NULL)
        return;
    inOrderTravel(root->left);
    printf("%d ", root->data);
    inOrderTravel(root->right);
}

int main()
{
    int T, n, q, val;
    scanf("%d", &T);

    while (T--)
    {
        scanf("%d", &n);
        Node *root = NULL;
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &val);
            root = insertBST(root, val);
        }
        scanf("%d", &q);
        for (int i = 0; i < q; i++)
        {
            scanf("%d", &val);
            Node *toDelete = search(root, val);
            if (toDelete != NULL)
            {
                root = deleteNode(root, toDelete);
            }
            preOrderTravel(root);
            printf("\n");
            inOrderTravel(root);
            printf("\n");
        }
    }

    return 0;
}