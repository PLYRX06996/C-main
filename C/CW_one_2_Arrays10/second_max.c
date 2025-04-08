#include<stdio.h>
#include<limits.h>
int main(){
    int n;
    printf("Enter the value for number of elements:");
    scanf("%d", &n);
    int arr[n];
    n=sizeof(arr)/4;
    int max, secMAX;
    for(int i=0;i<n;i++){
        printf("Enter the %dth element:", i+1);
        scanf("%d", &arr[i]);
    }
    max=INT_MIN;
    secMAX=INT_MIN;
    for(int j=0;j<n;j++){
        if(max<arr[j]) max=arr[j];
    }
    for(int k=0;k<n;k++){
        if(arr[k]!=max && arr[k]>secMAX) secMAX=arr[k];
    }
    printf("The max of all elements is:%d", max);
    printf("\nThe 2nd max of all elements is:%d", secMAX);
}
