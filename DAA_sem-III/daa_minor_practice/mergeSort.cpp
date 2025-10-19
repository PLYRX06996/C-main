#include<bits/stdc++.h>
using namespace std;
int main() {
    
    return 0;
}
void merge(vector<int>& arr, int l,  int m,  int h){
    int leftSize = m - l + 1;
    int rightSize = h - (m + 1) + 1;
    vector<int> leftArr(leftSize);
    vector<int> rightArr(rightSize);
    for(int i = 0; i < leftSize; i++){
        leftArr[i] = arr[l + i];
    }
    for(int j = 0; j < rightSize; j++){
        rightArr[j] = arr[m + 1 + j];
    }
    leftArr.push_back(INT_MAX);
    rightArr.push_back(INT_MAX);
    int c1 = 0, c2 = 0, c = l;
    while(c <= h){
        if(leftArr[c1] <= rightArr[c2]){
            arr[c++] = leftArr[c1++];
        } else{
            arr[c++] = rightArr[c2++];
        }
    }
}
void mergeSort(vector<int>& arr, int l, int h){
    if( l >= h) return;
    int m = (l + h)/2;
    mergeSort(arr, l, m);
    mergeSort(arr, m+ 1, h);
    merge(arr, l, m, h);
}