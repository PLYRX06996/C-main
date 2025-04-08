#include<stdio.h>
int fibobacci(int n){
    if(n>=1){ printf("0 ");
    if(n>=2){ printf("1 ");
    if(n>=3){
            int a=1, b=1;
        for(int i=3;i<=n;i++){
            printf("%d ",a);
        int next=a;
        a=a+b;
        b=next;
        }
    }
    }
    }
    return;
}
int main(){
    int n;
    printf("Enter the value for n:");
    scanf("%d", &n);
    printf("The first %d Fibonacci numbers are:",n);
    fibobacci(n);
    return 0;
}
