#include<iostream>
using namespace std;
void swap(int *a, int *b){
    int temp = *b;
        *b = *a;
        *a = temp;
}
void insertionSort(int arr[], int n){
    int min = arr[0];
    for(int i = 1; i < n; i++) {
        for(int j = i; j > 0; j--){
            if(arr[j+1] > arr[j]){
                swap(&arr[j+1], &arr[j]);
        }
    }
}
}
void selectionSort(int arr[], int n){
    
}
void printArray(int arr[], int n){
    for(int i = 0; i < n; i++){
        cout << arr[i] << " ";
    }
}
int main(){
    int n;
    cout << "Enter the number of elements for the Array:";
    cin >> n;
    int arr[n];
    cout << "Now enter all the elements one-by-one, seperating each by space.: ";
    for(int i = 0; i < n; i++){
        cin >> arr[i];
    }
    insertionSort(arr, n);
    printArray(arr, n);
    return 0;
}
