#include<bits/stdc++.h>
using namespace std;
int main() {
    
    return 0;
}
void swap(int *a, int *b){}
int partIndex(vector<int>& arr, int l, int h){
    int pivot = arr[h];
    int partI = l - 1;
    for(int i = l; i < h; i++){
        if(arr[i] <= pivot){
            swap(&arr[++partI], &arr[i]);
        }
    }
    swap(&arr[partI + 1], &arr[h]);
    return partI + 1;
}
void quickSort(vector<int>& arr, int l, int h){
    if( l >= h) return;
    int p = partIndex(arr, l, h);
    quickSort(arr, l, p - 1);
    quickSort(arr, p + 1, h);
}