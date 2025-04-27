#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<limits.h>
#include<string.h>

#define MAX_TABLE_SIZE 100005
#define MAX 31

typedef struct User {
    char fname[MAX], sname[MAX];
    char roll[MAX], course[MAX], hostel[MAX];
    char city[MAX];
    int sem;
} User;
User *HashTable;
int *isOccupied;
int tableSize;

int EncodeFirstThree(char *str) {
    int result = 0;
    int base = 1;

    for(int i = 0; i < 3; i++) {
        result = result + (str[i] - 'a') * base;
        base = base * 26;
    }
    return result;
}
int EncodeLastThree(char *str) {
    int len = strlen(str);
    int result = 0;
    int base = 1;

    for(int i = len - 3; i < len; i++) {
        result = result + (str[i] - 'a') * base;
        base = base * 26;
    }
    return result;
}
int getPrimaryHashkey(char *fname, char *sname, int tablesize) {
    int key = EncodeLastThree(fname) *  26 * 26 * 26 + EncodeLastThree(sname);
    return key % tablesize;
}
int getSecondryHashkey(char *fname, char *sname, int s) {
    int key = EncodeFirstThree(fname) *  26 * 26 * 26 + EncodeFirstThree(sname);
    return (key % s) + 1;
}
int insertUser(char *fname, char *sname, char *roll, char *course, char *hostel, char *city, int sem, int tablesize, int s) {
    int Pkey = getPrimaryHashkey(fname, sname, tablesize);
    int Skey = getSecondryHashkey(fname, sname, s);
    int probeCount = 0;
    int idx = Pkey;

    while(isOccupied[idx]) {
        idx = (Pkey + (++probeCount) * Skey) % tablesize;
    }
    strcpy(HashTable[idx].fname, fname);
    strcpy(HashTable[idx].sname, sname);
    strcpy(HashTable[idx].course, course);
    strcpy(HashTable[idx].roll, roll);
    strcpy(HashTable[idx].hostel, hostel);
    strcpy(HashTable[idx].city, city);
    HashTable[idx].sem = sem;
    isOccupied[idx] = 1;

    return probeCount;
}
int searchUser(char *fname, char *sname, int tablesize, int s) {
    int Pkey = getPrimaryHashkey(fname, sname, tablesize);
    int Skey = getSecondryHashkey(fname, sname, s);
    int probeCount = 0;
    int idx = Pkey;

    while(isOccupied[idx]) {
        if(strcmp(HashTable[idx].fname, fname) == 0 && strcmp(HashTable[idx].sname, sname) == 0){
            printf("%d\n", probeCount);
            printf("Roll: %s, Course: %s, Semester: %d, Hostel: %s, City: %s\n", HashTable[idx].roll, HashTable[idx].course, HashTable[idx].sem, HashTable[idx].hostel, HashTable[idx].city);
            return 1;
        }
        idx = (Pkey + (++probeCount) * Skey) % tablesize;
    }
    printf("%d\nnot found\n", probeCount);
    return 0;
}
int main() {
    int T;
    scanf("%d", &T);
    while(T--) {
        int n, s;
        scanf("%d %d %d", &n, &tableSize, &s);
        HashTable = (User*)malloc(tableSize * sizeof(User));
        isOccupied = (int*)calloc(tableSize, sizeof(int));
        for(int i = 0; i < n; i++) {
            User u;
            scanf("%s %s %s %s %d %s %s", u.fname, u.sname, u.roll, u.course, &u.sem, u.hostel, u.city);
            int NumOfprobes = insertUser(u.fname, u.sname, u.roll, u.course, u.hostel, u.city, u.sem, tableSize, s);
            printf("%d\n", NumOfprobes);
        }
        int q;
        scanf("%d", &q);
        for (int i = 0; i < q; i++) {
            char fname[MAX], sname[MAX];
            scanf("%s %s", fname, sname);
            searchUser(fname, sname, tableSize, s);
        }
        free(HashTable);
        free(isOccupied);
    }
    return 0;
}
// int deleteUser(char *fname, char *sname, int tablesize, int s) {
//     int Pkey = getPrimaryHashkey(fname, sname, tablesize);
//     int Skey = getSecondryHashkey(fname, sname, s);
//     int probeCount = 0;
//     int idx = Pkey;

//     while (isOccupied[idx]) {
//         if (strcmp(HashTable[idx].fname, fname) == 0 && strcmp(HashTable[idx].sname, sname) == 0) {
//             isOccupied[idx] = 0;
//             return idx; // Successfully deleted
//         }
//         idx = (Pkey + (++probeCount) * Skey) % tablesize;
//     }
//     return -1; // Not found
// }