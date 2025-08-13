#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

void input_array(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        scanf("%d", &arr[i]);
    }
}
void swap2(int *n1, int *n2)
{
    int temp = *n2;
    *n2 = *n1;
    *n1 = temp;
}
int partitionInd(int arr[], int l, int h)
{
    int pivot = arr[l];
    int i = l;
    int j = h;
    while (i < j)
    {
        while (arr[i] <= pivot && i < h)
        {
            i++;
        }
        while (arr[j] > pivot && j > l)
        {
            j--;
        }
        if (i < j)
            swap2(&arr[i], &arr[j]);
    }
    swap2(&arr[l], &arr[j]); // this is the problem
    return j;
}
// INfinite recursion
void quickSort(int arr[], int l, int h)
{
    if (l < h)
    {
        int partI = partitionInd(arr, l, h);
        quickSort(arr, l, partI - 1);
        quickSort(arr, partI + 1, h);
    }
}
void printArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
}
int main()
{
    int size;
    scanf("%d", &size);
    int arr[size];
    input_array(arr, size);
    quickSort(arr, 0, size);
    printArray(arr, size);
    return 0;
}