#include<stdio.h>
int main(){
    int a,b;
    printf("Enter the values for a and b: ");
    scanf("%d %d", &a, &b);
    /*int temp = a;
    a=b;
    b=temp;*/
    a=a+b;
    b=a-b;
    a=a-b;
    printf("The swapped value of a is %d and b is %d", a, b);
    return 0;
}
