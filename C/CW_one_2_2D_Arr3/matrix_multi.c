#include<stdio.h>
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
            printf("Enter a%d%d:", i+1, j+1);
            scanf("%d", &arr[i][j]);
        }
    }
    int r2, c2;
    printf("Enter the value for number of rows:");
    scanf("%d", &r2);
    printf("Enter the value for number of columns:");
    scanf("%d", &c2);
    printf("Enter the entries for 2nd matrix:%dX%d", r2, c2);
    printf("\n");
    int brr[r2][c2];
    for(int i=0;i<r2;i++){
        for(int j=0;j<c2;j++){
            printf("Enter b%d%d:", i+1, j+1);
            scanf("%d", &brr[i][j]);
        }
    }
    if(c1==r2){
    int crr[r1][c2];
    for(int i=0;i<r1;i++){
        for(int j=0;j<c2;j++){
                crr[i][j]=0;
            for(int k=0;k<c1;k++){
                crr[i][j]+=arr[i][k]*brr[k][j];
            }
        }
    }

    printf("\n");
    for(int i=0;i<r1;i++){
         for(int j=0;j<c2;j++){
            printf("%d ", crr[i][j]);
         } printf("\n");
      }
    } else printf("Please enter valid inputs for matrix multiplication.");
    return 0;
}
