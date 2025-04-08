#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define STRSIZE 20

struct {
    char name[STRSIZE];
    float salary;
    int age;
} p1, p2;
struct nametype{
    char first[STRSIZE];
    char mid[STRSIZE];
    char last[STRSIZE];
};
struct nametype name1, name2;
typedef struct{
    char first[STRSIZE];
    char mid[STRSIZE];
    char last[STRSIZE];
} NAMETYPE;
NAMETYPE name3, name4;
void main(){
    
}