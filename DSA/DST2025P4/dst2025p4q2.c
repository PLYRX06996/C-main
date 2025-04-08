#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
typedef struct Node
{
    int data;
    struct Node *next;
} Node;

Node *createNode(int value)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}
Node *insertAtEnd(Node *head, int value)
{
    Node *temp = createNode(value);
    if (head == NULL)
    {
        return temp;
    }
    Node *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = temp;
    return head;
}
bool checkLocalMinima(Node *head)
{
    if (head == NULL || head->next == NULL || head->next->next == NULL)
    {
        return false;
    }
    Node *current = head->next;
    Node *prev = head;
    Node *next;
    bool foundMinima = false;
    while (current->next != NULL)
    { // current->next->next != null nahi aayega
        next = current->next;
        if (current->data < current->next->data && current->data < prev->data)
        {
            printf("%d ", current->data);
            foundMinima = true;
        }
        prev = current;
        current = next;
    }
    if (foundMinima)
    {
        printf("\n");
    }
    return foundMinima;
}
void freeSLL(Node **head)
{
    Node *current = *head;
    while (*head != NULL)
    {
        Node *temp = current;
        current = (*head)->next;
        *head = current;
        free(temp);
    }
    *head = NULL;
}
int main()
{
    int tc;
    scanf("%d", &tc);

    while (tc--)
    {
        int n;
        scanf("%d", &n);
        Node *head = NULL;
        while (n--)
        {
            int a;
            scanf("%d", &a);
            head = insertAtEnd(head, a);
        }
        if (!checkLocalMinima(head))
        {
            printf("none\n");
        }
        freeSLL(&head);
    }

    return 0;
}