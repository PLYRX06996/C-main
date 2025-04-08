#include<stdio.h>
int sum(int n){
    if(n==1) return 1;
    int recAns=n+sum(n-1);
    return recAns;
}
void greet(int n){
    if(n==0) return ;
    printf("Good morning.\n");
    greet(n-1);
}
int main(){
    greet(1);
    int n;
    printf("Enter n:");
    scanf("%d", &n);
    int ans=sum(n);
    printf("%d", ans);
    return 0;
}
