#include<stdalign.h>
void main()
{
    int n;
    printf("Enter the value for making 'Odd Number Triangle' : ");
    scanf("%d", &n);
    int l = 1 + (n-1)*2;
    for(int i=1; i<=l;i=i+2){
        for(int j=1;j<=i;j=j+2){
            printf("%d ", j);
        } printf("\n");
    }
}


