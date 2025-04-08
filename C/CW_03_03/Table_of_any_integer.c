#include<stdio.h>
int main()
{
    int n;
    printf("Enter the value for n:");
    scanf("%d", &n);
   for(int i=n;i<=10*n;i=i+n){
    printf("%d\n", i);
   }
}
