#include<stdio.h>
#include<limits.h>
int main(){
    int r1, c1;
    printf("Enter the value for number of rows:");
    scanf("%d", &r1);
    printf("Enter the value for number of columns:");
    scanf("%d", &c1);
    printf("Enter the entries for 1st matrix:%dX%d", r1, c1);
    printf("\n");
    int arr[r1][c1];
    for(int i=0;i<r1;i++){
        for(int j=0;j<c1;j++){
            printf("Enter a%d%d:", i, j);
            scanf("%d", &arr[i][j]);
        }
    }
   int max=INT_MIN;
   int min=INT_MAX;

    for(int i=0;i<r1;i++){
    for(int j=0;j<c1;j++){
        if(max<arr[i][j]) max=arr[i][j];
        if(min>arr[i][j]) min=arr[i][j];
    }
    }
    printf("The max of all elements is:%d\n", max);
    printf("The min of all elements is:%d", min);
}
