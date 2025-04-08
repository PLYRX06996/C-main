#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
typedef struct Node
{
    struct Node *next;
    int data;
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
int returnValue(Node *head, int position)
{
    if (position < 1)
    {
        return INT_MIN;
    }
    Node *current = head;
    for (int i = 1; current != NULL && i < position; i++)
    {
        current = current->next;
    }
    if (current == NULL)
    {
        return INT_MIN;
    }
    return current->data;
}
Node *deleteNode(Node *head, int position)
{
    Node *temp = head;
    Node *prev = NULL;

    if (temp == NULL)
        return head;

    if (position == 1)
    {
        head = temp->next;
        free(temp);
        return head;
    }

    for (int i = 1; temp != NULL && i < position; i++)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp != NULL)
    {
        prev->next = temp->next;
        free(temp);
    }

    return head;
}
int largestEven(Node *head)
{
    Node *current = head;
    int max_even = INT_MIN;
    int pos = -1;
    int current_pos = 1;

    while (current != NULL)
    {
        if (current->data % 2 == 0)
        {
            if (current->data > max_even)
            {
                max_even = current->data;
                pos = current_pos;
            }
        }
        current = current->next;
        current_pos++;
    }

    return pos;
}

int largestOdd(Node *head)
{
    Node *current = head;
    int max_odd = INT_MIN;
    int pos = -1;
    int current_pos = 1;

    while (current != NULL)
    {
        if (current->data % 2 != 0)
        {
            if (current->data > max_odd)
            {
                max_odd = current->data;
                pos = current_pos;
            }
        }
        current = current->next;
        current_pos++;
    }

    return pos;
}
void printList(Node *head)
{
    while (head != NULL)
    {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}
void freeList(Node *head)
{
    Node *current = head;
    while (current != NULL)
    {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
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
        Node *newHead = NULL;
        while (1)
        {
            int posEven = largestEven(head);
            if (posEven != -1)
            {
                newHead = insertAtEnd(newHead, returnValue(head, posEven));
                head = deleteNode(head, posEven);
            }
            int posOdd = largestOdd(head);
            if (posOdd != -1)
            {
                newHead = insertAtEnd(newHead, returnValue(head, posOdd));
                head = deleteNode(head, posOdd);
            }
            if (head == NULL)
            {
                break;
            }
        }
        printList(newHead);
        freeList(newHead);
    }
    return 0;
}
