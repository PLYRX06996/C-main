#include<stdalign.h>
void main()
{
    int n;
    printf("Enter the value for making 'Odd Number Square' : ");
    scanf("%d", &n);
    int l = 1 + (n-1)*2;
    for(int i=1; i<=n;i++){
        for(int j=1;j<=l;j=j+2){
            printf("%d ", j);
        } printf("\n");
    }
}
