#include<stdio.h>
int main(){
    int n;
    printf("Enter the value for number of elements:");
    scanf("%d", &n);
    int arr[n];
    n=sizeof(arr)/4;

    for(int i=0;i<n;i++){
        arr[i]=i+1;
        printf("%d ", arr[i]*arr[i]);
    }
}


