#include<stdio.h>
int main(){
    int n;
    printf("Enter the value for number of elements:");
    scanf("%d", &n);
    int arr[n];
    n=sizeof(arr)/4;

    for(int i=0;i<n;i++){
        arr[i]=i+1;
        printf("%d ", arr[i]);
    }
    printf("\n");
    for(int j=0;j<n;j++){
        if(j%2!=0) arr[j]=arr[j]*2;
        else arr[j]+=10;
        printf("%d ", arr[j]);
    }
}
