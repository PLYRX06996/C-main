#include<stdio.h>
void main()
{
    int n;
    printf("Enter the value for making 'Alphabet Square': ");
    scanf("%d", &n);
    for(int i=1;i<=n;i++){
        for(char j=65;j<(65+n);j++){
            printf("%c ", j);
        } printf("\n");
    }
}
