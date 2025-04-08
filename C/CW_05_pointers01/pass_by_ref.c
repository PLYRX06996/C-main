#include<stdio.h>
int swap(int *x, int *y){
    int temp;
    temp=*x;
    *x=*y;
    *y=temp;
    return*x,*y;
}
int main(){
    int a,b;
    printf("Enter the values for a and b: ");
    scanf("%d %d", &a, &b);
    swap(&a, &b);
    printf("The swapped value of a is %d and b is %d", a, b);
    return 0;
}
void just(){
    int a = 5;//Integer data type
    int *ptr =&a;//pointer data type
    *ptr=7;
    //address of pointer
    printf("%p", &a);
    printf("\n%x\n%d", &ptr, *ptr);
    return;
}
