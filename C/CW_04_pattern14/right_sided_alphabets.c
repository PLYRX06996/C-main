#include<stdio.h>
void main()
{
    int n;
    printf("Enter a value for number of rows:");
    scanf("%d", &n);
    char a = 'A';
    for(int i=1;i<=n;i++){
        for (int j=1;j<=n;j++){
            if((i+j)<=n){
            printf(" ");
            } else {printf("%c", a);
            a++;
        }
        } printf("\n"); a=65;
    }
}
