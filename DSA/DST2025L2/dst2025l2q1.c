#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
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
Node* insertAtEnd(Node* head, int value){
  Node* temp = createNode(value);
  if(head == NULL){
    return temp;
  }
  Node* current = head;
  while(current->next != NULL){
    current = current->next;
  }
  current->next = temp;
  return head;
}
bool checkConsecutive(Node* head, int dataC){
  if(head == NULL || head->next == NULL)
  return false;
Node* current = head;
while (current->next != NULL) {
  if(current->data == dataC && current->next->data == dataC)
  return true;
current = current->next;
}
return false;
}
void freeList(Node* head){
  while(head != NULL){
    Node* temp = head;
    head = head->next;
    free(temp);
  }
  head = NULL;
}
int main() {
  int tc;
  scanf("%d", &tc);
  while(tc--){
    int n;
    scanf("%d", &n);
    Node* head = NULL;
    while (n--) {
    int a;
    scanf("%d", &a);
    head = insertAtEnd(head, a);
    }
    int q;
    scanf("%d", &q);
    if(checkConsecutive(head, q)){
      printf("yes\n");
    } else {
      printf("no\n");
    }
    freeList(head);
  }
    return 0;
}