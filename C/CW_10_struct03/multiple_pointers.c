#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef int* pointer;
int main(){
    int a=3,b=5;
    pointer x=&a,y=&b; //Incorporate int* x and int y;
    printf("%p\n%p", x, y);

    //typedef
    return 0;
}
