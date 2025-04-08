#include<stdio.h>
void main()
{
    int n;
    long long int product = 1;
    printf("Enter a number:");
    scanf("%d", &n);
    for(int i=1;i<=n;i++){
        product = product*i;
        printf("The factorial of %d is :%d\n",i, product);
    }
    //printf("The factorial value of %d is:%d", n, product);
}
