/*#include<stdio.h>
void main()
{
    int n;
    printf("Enter a value for number of rows:");
    scanf("%d", &n);
    int m=2*n-1;
    int a = 2;
    for(int i=1;i<=n;i++){
            a = 2*i;
        for (int j=1;j<=m;j++){
            if((i+j)<=n || (i+j)>=a+n){
            printf(" ");
            } else {printf("*");
        }
        } printf("\n");
        }
}*/
/*Method 2*/
#include<stdio.h>
void main()
{
    int n;
    printf("Enter a value for number of rows:");
    scanf("%d", &n);
    int nst=1, nsp = n-1;
    for(int i=1;i<=n;i++){
        for(int k=1;k<=nsp;k++){
            printf(" ");
        } nsp-=1;
        for (int j=1;j<=nst;j++){
            printf("*");
        } printf("\n"); nst+=2;
        }
}
