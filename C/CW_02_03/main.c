#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i = 65;
    char j = 'A';
    int a;
    char ch = 'a';
    if(i==j)
        printf("C is WOW");
    else
        printf("C is a headache");
    if(3+2%5)
        printf("\nThis works");
    if(a=10)
        printf("\nEven this works");
    if(-5)
        printf("\nSurprisingly even this also works.");
    if('a')
        printf("\n HELLO");
    if(ch)
        printf("\nHeLLO");
    if(ch='g')
        printf("\nHelLo");
    if(ch=='g'){
        printf("\nHello");
    }else{
        printf("\nThis is amazing");
    }
    return 0;
}
