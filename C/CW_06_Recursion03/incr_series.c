#include<stdio.h>
/*This method of Usingg Recursive function is called
Parameterised way.*/
int print(int x, int n){
    print(x+1,n);//code after and before call

    if(x==n+1) return;
    printf("%d\n", x);
    return ;
}
int main(){
    int n;
    printf("Enter the value for n:");
    scanf("%d", &n);
    int x=1;
    int num = print(x, n);
    return 0;
}
