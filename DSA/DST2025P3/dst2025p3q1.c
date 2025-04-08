#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef struct Node{
    int data;
    struct Node* next;
} Node;

Node* newNode(int value){
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = value;
    temp->next = NULL;
    return temp;
}

void insertAtPosition(Node** head, int pos, int value) {
    struct Node* temp = newNode(value);
    if(pos < 0) return;
    if (pos == 0) {
        temp->next = *head;
        *head = temp;
    } else {
        Node* prev = *head;
        if(prev == NULL) {
            temp->next = *head;
            *head = temp;
            return;
        }
        int count = 0;
        while(count < pos - 1 && prev->next != NULL) {
            prev = prev->next;
            count++;
        }
        
        temp->next = prev->next;
        prev->next = temp;
    }
}

void display(Node* head){
    if(head == NULL){
        printf("epmty\n");
        return;
    }
    while(head != NULL){
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

void freeList(Node* head){
    Node* temp;
    while(head != NULL){
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    int tc;
    scanf("%d", &tc);
    while(tc--){
        int n;
        scanf("%d", &n);
        Node* head = NULL;
        while(n--){
            int a, i;
            scanf("%d %d", &a, &i);
            insertAtPosition(&head, i, a);
        }
        display(head);
        freeList(head);
    }
    return 0;
}