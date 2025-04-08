#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct Node{
    int data;
    struct Node* next;
} Node;

Node* creatNode(int value){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

void insertAtEnd(Node** head, int a){
    Node* temp = creatNode(a);
    if(*head == NULL){
        *head = temp; 
    } else {
        Node* current = *head;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = temp;
    }
   return;
}

void delOfeveryI(Node** head, int i){
    if(*head == NULL || i < 1){
        return;
    }
    if(i == 1){
        Node* current = *head;
        while(current != NULL){
            Node* temp = current;
            current = current->next;
            free(temp);
        }
        *head = NULL;
        return;
    }
    Node* current = *head;
    Node* prev = NULL;
    int count = 1;
    while(current != NULL){
        if(count % i == 0){
            prev->next = current->next;
            free(current);
            current = prev->next;
        } else {
            prev = current;
            current = current->next;
        }
        count++;
    }
}

void displayOfRem(Node** head){
    Node* temp = *head;
    if(*head == NULL){
        printf("empty\n");
    } else {
        while(temp != NULL){
            printf("%d ", temp->data);
            temp = temp->next;
            } printf("\n");
    }
}
int main(){
    int tc;
    scanf("%d", &tc);
    while(tc--){
        Node* head = NULL;
        int n;
        scanf("%d", &n);
        while(n--){
            int a;
            scanf("%d", &a);
            insertAtEnd(&head, a);
        }
        int q;
        scanf("%d", &q);
        while(q--){
            int i;
            scanf("%d", &i);
            delOfeveryI(&head, i);
            displayOfRem(&head);
        }
    }
    return 0;
}