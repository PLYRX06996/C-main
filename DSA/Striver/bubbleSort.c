#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
/*Pushes max at last by adjacent swapping 
for each 1 outer-loop*/

void input_array(int (*arr)[], int size){
    for(int i = 0; i < size; i++){
        scanf("%d", &(*arr)[i]);
    }
}
void swap2(int* n1, int* n2){
    int temp = *n2;
        *n2 = *n1;
        *n1 = temp;
}
void bubbleSort(int arr[], int n){
    for(int i = 0; i < n; i++){
        int didSwap = 0;
        int j = 0;
        // while loop runs until till the condition becomes false
        // while(j < n - i - 1 && arr[j] > arr[j+1]){ // think abt j < n - i
        //     swap2(&arr[j], &arr[j+1]);
        //     didSwap = 1;
        //     j++;
        // }
        for(int j = 0; j < n - i; j++){
            if(arr[j] > arr[j+1]){
                swap2(&arr[j], &arr[j+1]);
                didSwap = 1;
            }
        }
        if(didSwap == 0) break;
    }
}
void printArray(int arr[], int n){
    for(int i = 0; i < n; i++){
        printf("%d ", arr[i]);
    }
}
int main(){
    int size;
    scanf("%d", &size);
    int arr[size];
    input_array(&arr, size);
    bubbleSort(arr, size);
    printArray(arr, size);
    return 0;
}