#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
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
Node* deleteOcuurance(Node *head, int dataC) {
    if (head == NULL || head->next == NULL) {
        return head;
    }
    
    Node *current = head;
    while (current != NULL && current->next != NULL) {
        if (current->data == dataC && current->next->data == dataC) {
            Node *temp = current->next;
            current->next = temp->next;
            free(temp);
        } else {
            current = current->next;
        }
    }
    return head;
}
void freeList(Node *head)
{
    while (head != NULL)
    {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
    head = NULL;
}
void printList(Node *head){
    Node *current = head;
    while(current != NULL){
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
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
        int q;
        scanf("%d", &q);
        head = deleteOcuurance(head, q);
        printList(head);
        freeList(head);
    }
    return 0;
}