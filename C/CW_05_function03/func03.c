#include<stdio.h>
int add(int a , int b){
    return a+b;
}
void main(){
    int a, b;
    printf("Enter a value for a: ");
    scanf("%d", &a);
    printf("Enter a value for b: ");
    scanf("%d", &b);
    int sum = add(a, b);
    printf("%d", sum);
}
