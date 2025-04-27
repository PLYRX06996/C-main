#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<limits.h>

#define left(i) (2*i + 1)
#define right(i) (2*i + 2)
#define parent(i) ((i-1) / 2)
#define MAX 100
typedef struct {
    int Data[MAX];
    int heapSize;
} MaxHeap;
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
void MaxHeapify(MaxHeap *H, int x) {
    int largest = x;
    int l = left(x);
    int r = right(x);

    if(l < H->heapSize && H->Data[l] > H->Data[largest]) largest = l;
    if(r < H->heapSize && H->Data[r] > H->Data[largest]) largest = r;

    if(largest != x) {
        swap(&H->Data[x], &H->Data[largest]);
        MaxHeapify(H, largest);
    }
}
void MakeHeap(MaxHeap *H, int arr[], int n) {
    H->heapSize = n;
    for(int i = 0; i < n; i++) {
        H->Data[i] = arr[i];
    }
    //call MaxHeapify() on all non-leaf nodes
    for(int  i = (n - 1)/2; i >= 0; i--)
    MaxHeapify(H, i);
}
void HeapSort(int arr[], int n) {
    MaxHeap H;
    MakeHeap(&H, arr, n);
    while(H.heapSize > 1) {
        swap(&H.Data[0], &H.Data[H.heapSize - 1]);
        H.heapSize--;
        MaxHeapify(&H, 0);
    }
    for(int i = 0; i < n; i++) {
        arr[i] = H.Data[i];
    }
}
void bubbleUp(MaxHeap *H, int positionIndex, int InsertValue) {
    H->Data[positionIndex] = InsertValue;
    while(positionIndex > 0 && H->Data[positionIndex] > H->Data[parent(positionIndex)]) {
        swap(&H->Data[positionIndex], &H->Data[parent(positionIndex)]);
        positionIndex = parent(positionIndex);
    }
}
void enqueue(MaxHeap *H, int key) {
    H->heapSize++;
    H->Data[H->heapSize - 1] = INT_MIN;
    bubbleUp(H, H->heapSize-1, key);
}
int dequeue(MaxHeap *H) {
    if(H->heapSize <= 0) return INT_MIN;
    int max = H->Data[0];
    swap(&H->Data[0], &H->Data[H->heapSize - 1]);
    MaxHeapify(H, 0);
    return max;
}
void printHeap(MaxHeap *H) {
    for(int i = 0; i < H->heapSize; i++) {
        printf("%d ", H->Data[i]);
    }
    printf("\n");
}
    int main() {
    
    return 0;
}