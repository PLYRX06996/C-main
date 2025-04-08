#include<stdio.h>
void main()
{
    int n;
    printf("Enter an integer:");
    scanf("%d", &n);
    int count = 0;
    while(n>0){
        n = n/10;
        count++;
    }
    printf("the digits in the given number is: %d", count);
}
