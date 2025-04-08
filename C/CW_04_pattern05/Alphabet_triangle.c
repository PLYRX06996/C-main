#include<stdio.h>
void main()
{
    int n;
    printf("Enter the value for making 'Alphabet Triangle': ");
    scanf("%d", &n);
    int a = n + 64;
    for(int i=65;i<=a;i++){
        for(char j=65;j<=i;j++){
            printf("%c ", j);
        } printf("\n");
    }
}
