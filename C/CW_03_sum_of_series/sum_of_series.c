#include<stdio.h>
void main()
{
    int n, sum = 0;
    printf("Enter a value for the number of terms:");
    scanf("%d", &n);
    for(int i=1;i<=n;i++){
        if(i%2==0)
            sum = sum - i;
        else sum = sum + i;
    }
    printf("The sum of the given series is:%d", sum);
    /*If the number of terms is even then the sum will be negative,
     and if the number of terms is odd then the sum will be Positive.*/

    //By this observation we will try to build a formula
    /*   if(n%2==0) sum = -n; else sum = n + 1;
    no need of for loop   */
}
