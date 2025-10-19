#include<bits/stdc++.h>
using namespace std;
int main() {
    
    return 0;
}
void selectionSort(vector<int>& arr){
    for(int i = 0; i < arr.size() - 1; i++){
        int minIndex = i;
        for(int j = i; j < arr.size() - 1; j++){
            if(arr[j] < arr[minIndex]){
                minIndex = j;
            }
            swap(arr[i], arr[minIndex]);
        }
    }
}