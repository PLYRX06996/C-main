#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100003
#define MAX 31

typedef struct User {
    char fname[MAX], sname[MAX];
    char roll[MAX], course[MAX], hostel[MAX];
    char city[MAX];
    int sem;
} User;
typedef struct Node {
    User data;
    struct Node *next;
} Node;
Node *HashTable[TABLE_SIZE];
int getHashIndex(char fname[], char sname[]) {
    char fullKey[65];
    strcpy(fullKey, fname);
    strcat(fullKey, sname);

    long long hash = 0;
    for (int i = 0; fullKey[i]; i++) {
        hash = (hash * 31 + (fullKey[i] - 'a' + 1)) % TABLE_SIZE;
    }
    return (int)hash;
}
void insertUser(char *fname, char *sname, char *roll, char *course, char *hostel, char *city, int sem) {
    int idx = getHashIndex(fname, sname);
    Node *newNode = (Node*)malloc(sizeof(Node));
    
    strncpy(newNode->data.fname, fname, MAX-1);
    newNode->data.fname[MAX-1] = '\0';
    
    strncpy(newNode->data.sname, sname, MAX-1);
    newNode->data.sname[MAX-1] = '\0';
    
    strncpy(newNode->data.roll, roll, MAX-1);
    newNode->data.roll[MAX-1] = '\0';
    
    strncpy(newNode->data.course, course, MAX-1);
    newNode->data.course[MAX-1] = '\0';
    
    strncpy(newNode->data.hostel, hostel, MAX-1);
    newNode->data.hostel[MAX-1] = '\0';
    
    strncpy(newNode->data.city, city, MAX-1);
    newNode->data.city[MAX-1] = '\0';
    
    newNode->data.sem = sem;
    newNode->next = HashTable[idx];
    HashTable[idx] = newNode;
}
void searchUser(char *fname, char *sname) {
    int idx = getHashIndex(fname, sname);
    Node *current = HashTable[idx];
    
    while (current) {
        if (strcmp(current->data.fname, fname) == 0 && 
            strcmp(current->data.sname, sname) == 0) {
            printf("Roll: %s, Course: %s, Semester: %d, Hostel: %s, City: %s\n",
                   current->data.roll, current->data.course, current->data.sem,
                   current->data.hostel, current->data.city);
            return;
        }
        current = current->next;
    }
    printf("not found\n");
}
void freeHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = HashTable[i];
        while (current) {
            Node *temp = current;
            current = current->next;
            free(temp);
        }
        HashTable[i] = NULL;
    }
}
int main() {
    int T;
    scanf("%d", &T);
    
    while (T--) {
        memset(HashTable, 0, sizeof(HashTable));
        int n;
        scanf("%d", &n);
        
        for (int i = 0; i < n; i++) {
            User u;
            scanf("%s %s %s %s %d %s %s", 
                  u.fname, u.sname, u.roll, u.course, &u.sem, u.hostel, u.city);
            insertUser(u.fname, u.sname, u.roll, u.course, u.hostel, u.city, u.sem);
        }    
        int q;
        scanf("%d", &q);
    
        for (int i = 0; i < q; i++) {
            char fname[MAX], sname[MAX];
            scanf("%s %s", fname, sname);
            searchUser(fname, sname);
        }        
        freeHashTable();
    }
    return 0;
}