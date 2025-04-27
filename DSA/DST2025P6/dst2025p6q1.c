#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#define m 101

typedef struct user {
    char fname[15];
    char sname[15];
    char rollnumber[20];
    char course[5];
    int sem;
    char city[10];
} user;
typedef struct Node {
    struct user data;
    struct Node* next;
} Node;
Node* HashTable[m];
int hash(char fname[], char sname[]) {
    long long h = 0;
    char key[25];
    strcpy(key, fname);
    strcat(key, sname);
    for(int i = 0; key[i]; i++) {
        h = (h * (key[i] - 'a' + 1)) % m;
    }
    return h;
}
void insertNode(char fname[], char sname[], char rollnumber[], char course[], int sem, char city[]) {
    int idx = hash(fname, sname);
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->data.fname, fname);
    strcpy(newNode->data.sname, sname);
    strcpy(newNode->data.rollnumber, rollnumber);
    strcpy(newNode->data.course, course);
    newNode->data.sem = sem;
    strcpy(newNode->data.city, city);
    newNode->next = HashTable[idx];
    HashTable[idx] = newNode;
}
void query(char fname[], char sname[]) {
    
}
int main() {

    
    return 0;
}
