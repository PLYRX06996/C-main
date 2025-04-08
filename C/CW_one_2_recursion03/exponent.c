#include<stdio.h>
int exp(int a, int b){
    if(b==0) return 1;
    int recAns=a*exp(a, b-1);
    return recAns;
}
int main(){
    int a, b;
    printf("Enter the value for a:");
    scanf("%d", &a);
    printf("Enter the value for b:");
    scanf("%d", &b);
    if(b >=0){
    int ans=exp(a, b);
    printf("The value of a^b=%d", ans);
    } else printf("Invalid entry.");
    return 0;
}
