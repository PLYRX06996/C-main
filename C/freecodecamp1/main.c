#include<stdio.h>
#include<stdlib.h>
int main()
{
    float n1, n2, n3, n4, n5;

    printf("Enter the marks of Math: ");
    scanf("%f", &n1);
    printf("Enter the marks of English: ");
    scanf("%f", &n2);
    printf("Enter the marks of Physics: ");
    scanf("%f", &n3);
    printf("Enter the marks of Chemistry: ");
    scanf("%f", &n4);
    printf("Enter the marks of Civil: ");
    scanf("%f", &n5);
    float p = (n1+n2+n3+n4+n5)/5;
    printf("The average percentage of marks of five subjects is: %f", p);
    return 0;
}
