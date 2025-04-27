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
int LengthLL(Node *head)
{
    if (head == NULL)
    {
        return 0;
    }
    Node *current = head;
    int length = 0;
    while (current != NULL)
    {
        current = current->next;
        length++;
    }
    return length;
}
bool searchEle(Node *head, int data)
{
    if (head == NULL)
    {
        return false;
    }
    Node *current = head;
    while (current != NULL)
    {
        if (current->data == data)
        {
            return true;
        }
        current = current->next;
    }
    return false;
}
Node *deleteHead(Node *head)
{
    if (head == NULL)
    {
        return head;
    }
    Node *temp = head;
    head = head->next;
    free(temp);
    return head;
}
Node *deleteLast(Node *head)
{
    if (head == NULL || head->next != NULL)
    { // didn't took head->next != NULL first
        free(head);
        return NULL; // and return null instead of head
    }
    Node *current = head;
    while (current->next->next != NULL)
    {
        current = current->next;
    }
    Node *temp = current->next;
    free(temp);
    current->next = NULL;
    return head;
}
Node *deleteKth(Node *head, int Kth)
{
    if (head == NULL)
    { // || head->next == NULL
        return NULL;
    }
    int Index = Kth - 1;
    if (Index == 0)
    {
        Node *temp = head;
        head = head->next;
        free(temp);
        return head;
    }
    Node *current = head;
    Node *temp = head;
    while (Index > 0 && temp != NULL)
    {
        current = temp;
        temp = temp->next;
        Index--;
    }
    if(temp != NULL){ /*Important beacuse if temp goes to last at null we can't delete that*/
        current->next = temp->next;
        free(temp);
    }
    return head;
}
Node *insertAtEnd(Node *head, int value)
{
    Node *temp = createNode(value);
    if (head == NULL)
        return temp;
    Node *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = temp;
    return head;
}
Node *insertAtPosition(Node *head, int value, int index)
{
    Node *temp = createNode(value);
    if (head == NULL)
        return temp;
    if (index < 0)
        return NULL;
    if(index == 0)
    {
        temp->next = head;
        head = temp;
        return head;
    }
    int IndexCount = 1;
    Node *current = head;
    while (IndexCount != index && current->next != NULL)
    {
        current = current->next;
        IndexCount++;
    }
    Node *next = current->next;
    current->next = temp;
    temp->next = next;
    return head;
}
void freeLL(Node **head)
{
    Node *current = *head;
    while (*head != NULL)
    {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

int main()
{
    printf("-------- CREATING A LINKED LIST --------\n");
    int tc;
    scanf("%d", &tc);
    Node *linkedlist = NULL;
    while (tc--)
    {
        int a;
        scanf("%d", &a);
        linkedlist = insertAtEnd(linkedlist, a);
    }
    printf("-------- INSERT AT POSITION --------\n");
    int n;
    scanf("%d", &n);
    while (n--)
    {
        int a, i;
        scanf("%d %d", &a, &i);
        linkedlist = insertAtPosition(linkedlist, a, i);
    }
    printf("-------- CREATING A LINKED LIST --------\n");
    printf("-------- SEARCH FOR AN ELEMENT BY VALUE --------\n");
    int ele;
    scanf("%d", &ele);
    searchEle(linkedlist, ele);
    return 0;
}