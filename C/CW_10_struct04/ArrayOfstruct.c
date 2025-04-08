#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    typedef struct Books{
        char name[25 ];
        int noOfpages;
        float price;
    }book;

    book arr[3];
    //How to input values in Array and How it will construct
    strcpy(arr[0].name,"J.D.Lee");
    arr[0].noOfpages=1000;
    arr[0].price=800;

    strcpy(arr[1].name,"N.A.Physical Chemistry");
    arr[1].noOfpages=800;
    arr[1].price=400;

    strcpy(arr[2].name,"V.K.Jaiswal");
    arr[2].noOfpages=700;
    arr[2].price=350;

    for(int i=0;i<3;i++){
    printf("Name of the Book:%s\n", arr[i].name);
    printf("Number of pages in book:%d\n", arr[i].noOfpages);
    printf("Price of the book:%f\n", arr[i].price);
    printf("\n");
    }
    return 0;
}
