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

int StringToInt(char *str) {
    int len = strlen(str);
    int result = 0;
    int base = 1;
    for(int i = len - 1; i >= len - 3 && i >= 0; i--) {
        result = result + (str[i] - 'a') * base;
        base = base * 26;
    }
    return result;
}
int getLinearHashIndex(char *fname, char *sname, int tableSize) {
    int key = StringToInt(fname) * 26 * 26 * 26 + StringToInt(sname);// 26*26*26 is for hackerRank question
    return key % tableSize;// coz we have to demostrate LenearProbing so we have to make collisions
}// else we will be taking 100000
int insertUserLinearly(char *fname, char *sname, char *roll, char *course, char *hostel, char *city, int sem, int tablesize) {
    int probeCount = 0;
    int idx = getLinearHashIndex(fname, sname, tablesize);

    while (isOccupied[idx]) {
        idx = (idx + 1) % tablesize;
        probeCount++;
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
int searchUser(char *fname, char *sname, int tablesize) {
    int idx = getLinearHashIndex(fname, sname, tablesize);
    int probeCount = 0;

    while(isOccupied[idx]) {
        
        if(strcmp(HashTable[idx].fname, fname) == 0 && strcmp(HashTable[idx].sname, sname) == 0){
            printf("%d\n", probeCount);
            printf("Roll: %s, Course: %s, Semester: %d, Hostel: %s, City: %s\n", HashTable[idx].roll, HashTable[idx].course, HashTable[idx].sem, HashTable[idx].hostel, HashTable[idx].city);
            return 1;
        }
        idx = (idx + 1) % tablesize;
        probeCount++;
    }
    printf("%d\nnot found\n", probeCount);
    return 0;
}
int main() {
    int T;
    scanf("%d", &T);
    
    while (T--) {
        int n;
        scanf("%d %d", &n, &tableSize);
        
        HashTable = (User*)malloc(tableSize * sizeof(User));
        isOccupied = (int*)calloc(tableSize, sizeof(int));
        
        for (int i = 0; i < n; i++) {
            User u;
            scanf("%s %s %s %s %d %s %s", 
                  u.fname, u.sname, u.roll, u.course, &u.sem, u.hostel, u.city);
            int NumOfprobes = insertUserLinearly(u.fname, u.sname, u.roll, u.course, 
                                  u.hostel, u.city, u.sem, tableSize);
            printf("%d\n", NumOfprobes);
        }
        int q;
        scanf("%d", &q);
        for (int i = 0; i < q; i++) {
            char fname[MAX], sname[MAX];
            scanf("%s %s", fname, sname);
            searchUser(fname, sname, tableSize);
        }
        free(HashTable);
        free(isOccupied);
    }
    return 0;
}
// int deleteUser(char *fname, char *sname, int tablesize) {
//     int idx = getLinearHashIndex(fname, sname, tablesize);
//     int probeCount = 0;

//     while (isOccupied[idx]) {
//         if (strcmp(HashTable[idx].fname, fname) == 0 && strcmp(HashTable[idx].sname, sname) == 0) {
//             isOccupied[idx] = 0;
//             return idx; 
//         }
//         idx = (idx + 1) % tablesize;
//         probeCount++;
//     }
//     return -1; 
// }