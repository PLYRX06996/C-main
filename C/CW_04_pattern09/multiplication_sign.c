#include<stdio.h>
void main()
{
    int n;
    printf("Enter a value for number of rows:");
    scanf("%d", &n);
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            if((i+j==n+1)||(i==6j)){
                printf("*");
            }else printf(" ");

        } printf("\n");
    }

}
