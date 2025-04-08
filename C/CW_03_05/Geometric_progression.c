#include<stdio.h>
#include<math.h>
int main()
{
    int a,r, n;
    printf("Enter the values for first term, number of terms and common multiplier for a G.P.:\n");
    scanf("%d%d%d", &a, &n, &r);
    printf("Asked Geometric Progression is:");
    for(int i=1;i<=n;i++){
        printf("%d ", a);
        a=a*r;
    }
}
