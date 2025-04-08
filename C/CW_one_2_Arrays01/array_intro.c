#include<stdio.h>
int main()
{
    int n;
    printf("Enter the argument of array:");
    scanf("%d", &n);
    int arr[n];
    int m=sizeof(arr);
    printf("%d\n", m);
    for(int i=0; i<n; i++)
    {
        printf("Enter the %dth element:", i+1);
        scanf("%d", &arr[i]);
    }
    for(int j=0;j<n;j++){
        printf("The %dth element of Array is:%d\n", j+1, arr[j]);
    }
    return 0;
}
