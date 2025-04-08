#include<stdio.h>
int factorial(int n){
    int next=1;
    printf("1 ");
    for(int i=1;i<=n;i++){
        next=next*i;
        printf("%d ", next);
    }
    return next;
}
int main(){
    int n;
    printf("Enter the value for factorials up to n+1 numbers:");
    scanf("%d", &n);
    if(n>=0){
    printf("The factorials of first %d number(s) is/are:", n+1);
    factorial(n);
    } else printf("Enter valid Inputs.");
    return 0;
}
