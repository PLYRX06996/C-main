// Split the sorted data into two half --> into two half divide
// Everything b/w low and high = search space
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void input_array(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        scanf("%d", &arr[i]);
    }
}
int binarySearch(int arr[], int size, int tgt)
{
    int l = 0;
    int h = size - 1;
    
    while (l <= h)
    {
        int m = (int)(l + h) / 2;//this was the problem
        if (arr[m] == tgt)
            return m;
        else if (arr[m] > tgt)
        {
            h = m - 1;
        }
        else
            l = m + 1;
    }
    return -1;
}

int main()
{
    int size;
    scanf("%d", &size);
    int arr[size];
    input_array(arr, size);
    int tgt;
    printf("Enter the target element: ");
    scanf("%d", &tgt);
    printf("The index of the called element is: %d", binarySearch(arr, size, tgt)+1);
    return 0;
}