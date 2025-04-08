#include<stdio.h>
void main(){
    int n;
    printf("Enter the value for number of elements:");
    scanf("%d", &n);
    int arr[n];
    for(int i=0;i<n;i++){
        printf("Enter the %dth element:", i+1);
        scanf("%d", &arr[i]);
    }
    /*int brr[n];
    for(int j=0;j<n;j++){
    brr[j]=arr[n-1-j];
    }
    for
    (int k=0;k<n;k++){
        printf("%d ", brr[k]);
    }*/
    //Method 2 for solving w/o using extra array
    int i=0;
    int j=n-1;
    while(i<j){
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
        i++;
        j--;
    }
    for(int i=0;i<n;i++){
        printf("%d ", arr[i]);
    }
}
