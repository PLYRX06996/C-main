#include<stdio.h>
int print(int n){
    if(n==0) return;
    printf("%d\n", n);//code before call
    print(n-1);
    printf("%d\n", n);//code after call
    return ;
}
int main(){
    int n;
    printf("Enter the value for n:");
    scanf("%d", &n);
    int num = print(n);
}
    return 0;
