#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
/*It always takes an element and places it in its correct position
*/
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
void insertionSort(int arr[], int n){
    for(int i = 1; i < n; i++){ // think abt i <= n
        int j = i;
        while(j > 0 && arr[j] < arr[j-1]){ // think abt j >= 0
            swap2(&arr[j], &arr[j-1]);
            j--;
        }
        // for(int j = i; j >= 0; j--){
        //     if(arr[j] < arr[j-1]){
        //         swap2(&arr[j], &arr[j-1]);
        //     }
        // }
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
    insertionSort(arr, size);
    printArray(arr, size);
    return 0;
}