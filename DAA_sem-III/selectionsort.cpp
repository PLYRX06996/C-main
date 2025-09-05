#include<bits/stdc++.h>
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

void selectionSort(vector<int>& arr){
    int n = arr.size();
    for(int i = 0; i < n - 1; i++){
        int min = i;
        for(int j = i; j < n; j++){
            if(arr[min] > arr[j])
            min = j;
        }
        swap(&arr[min], &arr[i]);
    }
}

void printArray(vector<int>& arr){
    for(int num : arr){
        cout << num << " ";
    }
    cout << endl;
}

int main() {
    int size;
    cin >> size;
    vector<int> arr(size);
    insertArray(arr, size);
    selectionSort(arr);
    printArray(arr);
    return 0;
}