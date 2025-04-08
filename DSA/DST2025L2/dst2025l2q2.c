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
int checkConsecutive(Node *head, int dataC)
{
    if (head == NULL || head->next == NULL)
        return -1;
    Node *current = head;
    int countOcuur = 0;
    int maxOccur = 0;
    while (current != NULL)
    {
        if (current->data == dataC)
        {
            countOcuur++;
            if (countOcuur > maxOccur){
                maxOccur = countOcuur;
            }
        }
        else
        {
            countOcuur = 0;
        }
        current = current->next;
    }
    return maxOccur;
}
int startIndex(Node *head, int dataC){
    int max_occur = checkConsecutive(head, dataC);
    if(max_occur == -1){
        return -1;
    }
    int max_start_Ind = -1;
    int count_occur = 0;
    int current_Ind = -1;
    Node *current = head;
    while(current != NULL){
        current_Ind++;
        if(current->data == dataC){
            count_occur++;
            if(count_occur == 1){
                max_start_Ind = current_Ind;
            }
        } else{
            count_occur = 0;
        }
        if(count_occur == max_occur){
            return max_start_Ind;
        }
        current = current->next;
    }
    return -1;
}
int endIndex(Node *head, int dataC){
    int start_index = startIndex(head, dataC);
    int max_occur = checkConsecutive(head, dataC);
    if(start_index != -1){
    return start_index + max_occur - 1;
    }
    return -1;
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
        // printf("%d\n", checkConsecutive(head, q));
        if(startIndex(head, q) != -1)
        printf("%d %d\n", startIndex(head, q), endIndex(head, q));
        else printf("-1\n");
        freeList(head);
    }
    return 0;
}