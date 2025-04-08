#include<stdio.h>
int main(){
    int n;
    printf("Enter the value for number of elements:");
    scanf("%d", &n);
    int arr[n];
    n=sizeof(arr)/4;
    int product=1;
    for(int i=0;i<n;i++){
        printf("Enter the %dth element:", i+1);
        scanf("%d", &arr[i]);
        product=product*arr[i];
    }
    printf("The product of all elements of array is:%d", product);
}
