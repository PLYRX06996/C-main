#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef int Integer;
typedef float realNum;
int main(){
    realNum x=3.1415926;
    printf("%f", x);

    typedef struct Books{
        char name[20];
        int noOfpages;
        float price;
    }book;

    book b1, b2, b3, b4;
    b1.noOfpages=400;
    b1.price=650.45;
    strcpy(b1.name,"The Secret");

    printf("%d\n", b1.noOfpages);
    printf("%f\n", b1.price);
    printf("%s\n", b1.name);
    return 0;
}
