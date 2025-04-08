#include<stdio.h>
void main()
{
    int n;
    printf("Enter a value for number of rows:");
    scanf("%d", &n);
    int a = 1;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=i;j++){
            printf("%d ", a);
            a=a+1;
        } printf("\n");
    }
}
