#include<stdio.h>
int main(){
    int n;
    printf("Enter the value for number of elements:");
    scanf("%d", &n);
    int arr[n];
    n=sizeof(arr)/4;

    for(int i=0;i<n;i++){
        printf("Enter the %dth element:", i+1);
        scanf("%d", &arr[i]);
    }
    int x;
    printf("Enter the value for x:");
    scanf("%d", &x);
    int count=0;
    for(int j=0;j<n;j++){
        if(arr[j]>x) count++;
    }
    printf("%d", count);
}
