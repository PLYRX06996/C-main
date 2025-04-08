#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

// decay of an array i.e; arr == &arr[0]
/*  Basic Array Parameter   */
// Already an array declaration
// void func(int arr[])
// void func(int* arr)
// void func(int arr[n]) Though size is ignored in parameters
/*  Pointer to an Array */
// void func(int (*arr)[10])--> pointer to an array of 10 integers
// void func(int *arr[])

/*Select min and swap*/

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
void selectionSort(int arr[], int n){
    for(int i = 0; i < n - 1; i++){
        int min = i;
        for(int j = i; j < n - 1; j++){
            if(arr[j] < arr[min]){
                min = j;
            }
        }
            swap2(&arr[min], &arr[i]);
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
    selectionSort(arr, size);
    printArray(arr, size);
    return 0;
}