#include<stdalign.h>
#include<stdio.h>
void main(){
    /*2D Arrays can be called as Array of Arrays */
    int arr[3][3]={{11,12,13}, {14,15,16}, {17,18,19}};
    int brr[3][3]={1,2,3,4,5,6,7,8,9};

      for(int i=0;i<3;i++){
         for(int j=0;j<3;j++){
            printf("%d ", arr[i][j]);
         } printf("\n");
      }

      printf("\n");

      for(int i=0;i<3;i++){
         for(int j=0;j<3;j++){
            printf("%d ", brr[i][j]);
         } printf("\n");
      }

      printf("\n");

      for(int i=0;i<3;i++){
         for(int j=0;j<3;j++){
            printf("%d ", arr[i][j]+brr[i][j]);
         } printf("\n");
      }

}
