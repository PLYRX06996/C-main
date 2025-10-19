#include<bits/stdc++.h>
using namespace std;

#define left(i) 2*i
#define right(i) 2*i + 1
#define parent(i) i/2
typedef struct {
    vector<int> heap;
    int heapSize = 0;
} MaxHeap;

void maxHeapify(MaxHeap *H, int x){
    int largest = x;
    int l = left(x);
    int r = right(x);

    if(l <= H->heapSize && H->heap[l] > H->heap[largest])
        largest = l;
    if(r <= H->heapSize && H->heap[r] > H->heap[largest])
        largest = r;

    if(largest != x){
        swap(H->heap[x], H->heap[largest]);
        
        maxHeapify(H, largest);
}
}
void buildMaxHeap(MaxHeap *H, vector<int>& arr){
    H->heapSize = arr.size() - 1;
    H->heap = arr;
    for(int i = H->heapSize/2; i >= 1; i--){
        maxHeapify(H, i);
    }
}
void heapSort(vector<int>& arr){
    MaxHeap *H = new MaxHeap();
    buildMaxHeap(H, arr);
    while(H->heapSize > 1){
        swap(H->heap[1], H->heap[H->heapSize]);
        H->heapSize--;
        maxHeapify(H, 1);
    }
    arr = H->heap;
    delete H;
}

int main() {
    
    return 0;
}
