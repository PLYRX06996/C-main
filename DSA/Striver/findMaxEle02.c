#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
void input_array(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        scanf("%d", &arr[i]);
    }
}
void findMax(int arr[], int l, int h, int* maxIndex, int* maxValue){
    if(l == h){
        if(arr[l] > *maxValue){
            *maxIndex = l;
            *maxValue = arr[l];
        }
        return;
    }
    int m = (int)(l+h)/2;
    findMax(arr, l, m, maxIndex, maxValue);
    findMax(arr, m+1, h, maxIndex, maxValue);
}
int main(){
    int size;
    scanf("%d", &size);
    int arr[size];
    input_array(arr, size);
    int maxIndex = 0;
    int maxValue = arr[0];
    findMax(arr, 0, size-1, &maxIndex,  &maxValue);
    printf("Maximum element is at index %d with value %d\n", maxIndex, maxValue);
    return 0;
}