#include<stdalign.h>
#include<stdio.h>
void main(){
    /*2D Arrays can be called as Array of Arrays */
    int arr[3][3]={{1,2,3}, {4,5,6}, {7,8,9}};
    /*1 2
      3 4
      5 6
      7 8   4 rows 2 columns*/
    int brr[3][3]={1,2,3,4,5,6,7,8,9};
    /*1 2
      3 4
      5 6
      7 8   4 rows 2 columns*/
      for(int i=0;i<3;i++){
         for(int j=0;j<3;j++){
            printf("%d ", arr[i][j]);
         } printf("\n");
      }
      printf("\n");
      for(int i=0;i<3;i++){
         for(int j=0;j<3;j++){
            printf("%d ", brr[j][i]);
         } printf("\n");
      }
      int crr[5][5];
      for(int i=0;i<5;i++){
         for(int j=0;j<5;j++){
            crr[i][j]=10;
      }
      }
      for(int i=0;i<5;i++){
         for(int j=0;j<5;j++){
            printf("%d ", crr[j][i]);
         } printf("\n");
      }

}
