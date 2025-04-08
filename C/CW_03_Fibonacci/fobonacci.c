#include<stdio.h>
void main()
{
    //The below code is to print nth Fibonacci term
    int term1 = 0, term2 = 1, term3, n, m;
        printf("Enter the term number for Fibonacci series:");
        scanf("%d", &n);
        for (int i=1;i<n;i++){
            term3 = term2 + term1;
            term1 = term2;
            term2 = term3;
        }
        printf("%d\n", term3);
        //The below code is for printing first n Fibonacci terms
        term1 = 0, term2 = 1, term3, n, m;
        printf("Enter the number of terms for Fibonacci series:");
        scanf("%d", &n);
        for (int i=1;i<=n;i++){
            term3 = term2 + term1;
            term1 = term2;
            term2 = term3;
            printf("%d\t", term1);
        }
}
