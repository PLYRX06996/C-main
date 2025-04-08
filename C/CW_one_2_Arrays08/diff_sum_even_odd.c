#include<stdio.h>
int main(){
    int n;
    printf("Enter the value for number of elements:");
    scanf("%d", &n);
    int arr[n];
    n=sizeof(arr)/4;

    for(int i=0;i<n;i++){
        arr[i]=i+1;
    }
    int es=0, os=0;
    for(int j=0;j<n;j++){
        if(j%2!=0) os+=arr[j];
        else es+=arr[j];
    }
    printf("The diff b/w Even ele. and Odd ele.:%d ", es-os);
}
