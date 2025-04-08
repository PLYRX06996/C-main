#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
typedef struct Node
{
    int data;
    struct Node* next;
} Node;
Node* createNode(int value)
{
    Node* newNode = (Node* )malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}
Node* insertAtEnd(Node* head, int value)
{
    Node* temp = createNode(value);
    if (head == NULL)
    {
        return temp;
    }
    Node* current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = temp;
    return head;
}
void findMaxElement(Node* head)
{
    if (head == NULL)
    {
        printf("none\n");
        return;
    }
    int maxValue = INT_MIN;
    int maxIndex = -1;
    int currentINdex = 0;
    Node* current = head;
    while (current != NULL)
    {
        if (current->data > maxValue)
        {
            maxValue = current->data;
            maxIndex = currentINdex;
        }
        current = current->next;
        currentINdex++;
    }
    printf("Maximum element is at index %d with value %d\n", maxIndex, maxValue);
}
int main()
{
    Node* head = NULL;
    int n;
    scanf("%d", &n);
    while (n--)
    {
        int a;
        scanf("%d", &a);
        head = insertAtEnd(head, a);
    }
    findMaxElement(head);
    return 0;
}