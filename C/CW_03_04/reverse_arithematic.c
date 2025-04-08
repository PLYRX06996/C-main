#include<stdio.h>
void main()
{
    int a, d, n;
    if(d!=0 && d!=1 && n>0){
    printf("enter the value for the first term of A.P.:");
    scanf("%d", &a);
    printf("enter the value for the common difference of A.P.:");
    scanf("%d", &d);
    printf("enter the value for the number of terms of A.P.:");
    scanf("%d", &n);
    for(int i=0;i<=n;i++){
            if(a+i*d >0){

         printf("%d\n", a+i*d);
            }
    }
    } else {
        printf("Invalid Inputs");
    }

}
