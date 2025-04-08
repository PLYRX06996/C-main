#include<stdio.h>
void main(){
    int minimum();
    int n;
    printf("Enter a value for number of rows: ");
    scanf("%d", &n);
    int min=0; int a,b;
    for(int i=1;i<=2*n-1;i++){
        for(int j=1;j<=2*n-1;j++){
            a=i;
            if(i>n) a=2*n-i;
            b=j;
            if(j>n) b=2*n-j;
            min = minimum(a, b);

            /*if(a<b) min=a;
            else min=b;*/
            printf("%d", n+1-min);
        } printf("\n");
    }
}
int minimum(int a, int b){
    int min=b;
    if(a<b) min=a;
    return min;
}
