#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

void insertAtEnd(Node **head, int x)
{
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->data = x;
    if (*head == NULL)
    {
        temp->next = temp;
        *head = temp;
    }
    else
    {
        Node *curr = *head;
        while (curr->next != *head)
            curr = curr->next;
        curr->next = temp;
        temp->next = *head;
    }
}

void printList(Node *head)
{
    if (!head)
        return;
    Node *curr = head;
    do
    {
        printf("%d -> ", curr->data);
        curr = curr->next;
    } while (curr != head);
    printf("(head)\n");
}
