#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define N 1000

typedef struct Node{
    char name;
    struct Node* next;
} Node;

Node* creatNode(char ch){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->name = ch;
    newNode->next = NULL;
    return newNode;
}

void insertSAtEnd(Node** head, char value){
    Node* temp = creatNode(value);
    if(*head == NULL){
        return;
    }
    Node* current = *head;
    while(current->next != NULL){
        current = current->next;
    }
    current->next = temp;
}



int main(){
    int tc;
    scanf("%d", &tc);
    while(tc--){
        int nstud;
        Node* head = NULL;
        scanf("%d", &nstud);
        for(int i = 0; i < nstud; i++){
            char name;
            scanf("%c", &name);
            insertSAtEnd(&head, name);
        }
        int nofp;
        scanf("%d", &nofp);
        while(nofp--){
            char f1, f2;
            scanf("%c %c", &f1, &f2);
        }
    }
    return 0;
}