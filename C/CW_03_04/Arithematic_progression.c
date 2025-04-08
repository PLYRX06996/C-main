#include<stdio.h>
int main()
{
    int n, a, d;
    printf("Enter the first term, numbers of terms & common difference for the A.P.:\n");
    scanf("%d%d%d", &a, &n, &d);
    printf("Asked progression:");
    /*for(int i=a;i<=a+(n-1)*d;i=i+d){
        printf("%d ", i);}*/
    for(int i=1;i<=n;i++){
        printf("%d ", a+(i-1)*d);
    }


}
