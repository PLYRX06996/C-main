#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

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
void freeSLL(Node **head)
{
    while (*head != NULL)
    {
        Node *current = *head;
        Node *temp = (*head)->next;
        *head = temp;
        free(current);
    }
}
bool checkSorting(Node *head)
{

    if (head == NULL || head->next == NULL)
        return true;
    Node *current = head;
    while (current->next != NULL)
    {
        if (current->data >= current->next->data)
        {
            return false;
        }
        current = current->next;
    }
    return true;
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
        // if(checkSorting)){
        //     printf("yes\n");
        // } else printf("no\n");
        bool isSorted = checkSorting(head);
        if (isSorted)
        {
            printf("yes\n");
        }
        else
        {
            printf("no\n");
        }
        freeSLL(&head);
    }

    return 0;
}