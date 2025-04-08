#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include<stdbool.h>
typedef struct Node{
    int data;
    struct Node* next;
} Node;
Node* createNode(int value){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}
bool checkSorted(Node* head){
    if(head == NULL) return false;
    Node* current = head;
    while(current->next != NULL){
        if(current->data > current->next->data)
        return false;
        current = current->next;
    }
    return true;
}
Node* insertAtEnd(Node* head, int value){
    if(head == NULL) return createNode(value);
    Node* current = head;
    while(current->next != NULL){
        current = current->next;
    }
    Node* temp = createNode(value);
    current->next = temp;
    return head;
}
int main() {
    int tc;
    scanf("%d", &tc);
    while(tc--){
      int n;
      scanf("%d", &n);
      Node* head = NULL;
      while(n--){
        int d;
        scanf("%d", &d);
        head = insertAtEnd(head, d);
      }
      if(checkSorted(head))
        printf("yes\n");
        else printf("no\n");  
    }
    return 0;
}
