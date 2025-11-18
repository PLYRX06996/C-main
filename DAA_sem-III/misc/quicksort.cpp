#include<bits\stdc++.h>
using namespace std;

void insertArray(vector<int>& arr, int size) { 
    for(int i = 0; i < size; i++){
        cin >> arr[i];
    }
}

void swap(int *a, int *b){
    int temp = *b;
    *b = *a;
    *a = temp;
}

int partitionInd(vector<int>& arr, int l, int h){
    int pivot = arr[h];
    int partI = l - 1;

    for(int i = l; i < h; i++) {
        if(arr[i] <= pivot){
            swap(&arr[++partI], &arr[i]);
        }
    }
    swap(&arr[partI + 1], &arr[h]);
    return partI + 1;
}

void quickSort(vector<int>& arr, int l ,int h){
    if(l >= h) return;
    int partI = partitionInd(arr, l, h);
    quickSort(arr, l, partI - 1);
    quickSort(arr, partI + 1, h);
}
/*Best case: O(nlog(n))
Worst case: O(n^2)*/

void printArray(vector<int>& arr){
    for(int num : arr){
        cout << num << " ";
    }
    cout << endl;
}

int main(){
    int size;
    cin >> size;
    vector<int> arr(size);
    insertArray(arr, size);
    quickSort(arr, 0, size - 1);
    printArray(arr);
    return 0;
}