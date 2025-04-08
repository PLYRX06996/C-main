#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
void findMinMax(int arr[], int start, int end, int* min_ptr, int* max_ptr){
    if(start == end){
        *min_ptr = *max_ptr = arr[start];
        return;
    }
    if(end - start == 1){
        if(arr[end] < arr[start]){
            *min_ptr = arr[end];;
            *max_ptr = arr[start];
        }
        else {
            *min_ptr = arr[start];
            *max_ptr = arr[end];
        }
        return;
    }
    int mid = (int)(start + end)/2;
    int left_min, left_max, right_min, right_max;
    findMinMax(arr, start, mid, &left_min, &left_max);
    findMinMax(arr, mid+1, end, &right_min, &right_max);
    *min_ptr = (left_min < right_min) ? left_min : right_min;
    *max_ptr = (left_max > right_max) ? left_max : right_max;
}
int main(){
    int arr[MAX_SIZE];
    input_arrray(arr);
    int size = (sizeof(arr)) / sizeof(arr[0]);
    int min, max;
    findMinMax(arr, 0, size-1, &min, &max);
    
    return 0;
}