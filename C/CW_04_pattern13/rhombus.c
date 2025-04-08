#include<stdio.h>
void main()
{
    int n;
    printf("Enter a value for number of rows:");
    scanf("%d", &n);
    int m=2*n-1;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            if((i+j)<=n || (i+j)>(2*n))
                printf(" ");
            else printf("* ");
        } printf("\n");
    }
}
