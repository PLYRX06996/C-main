#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
// recursive Algorithm
// Divide and merege
// Step 1: Divide Array in two parts m = (l+h)/2
// Solved and conquer
void inputArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }
}
void merege2(int arr[], int l, int m, int h)
{
    int sizeleft = m - l + 1;
    int sizeright = h - (m + 1) + 1;
    int L[sizeleft], R[sizeright];
    for (int i = 0; i < sizeleft; i++)
    {
        L[i] = arr[l + i];
    }
    for (int i = 0; i < sizeright; i++)
    {
        R[i] = arr[m + 1 + i];
    }
    L[sizeleft] = INT_MAX;
    R[sizeright] = INT_MAX;
    int c1 = 0, c2 = 0, c = l;
    while (c <= h)
    {
        if (L[c1] < R[c2])
        {
            arr[c++] = L[c1++];
        }
        else
        {
            arr[c++] = R[c2++];
        }
    }
}
void mergeSort(int arr[], int l, int h)
{
    if (l >= h)
        return;
    int m = (int)(l + h) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, h);
    merege2(arr, l, m, h);
}
void displayArray(int arr[], int n)
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
    int arrO[size];
    inputArray(arrO, size);
    mergeSort(arrO, 0, size - 1);
    displayArray(arrO, size);
    return 0;
}