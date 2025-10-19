#include<bits/stdc++.h>
using namespace std;
int main() {
    
    return 0;
}
void insertionSort(vector<int>& arr){
    for(int i = 1; i < arr.size() - 1; i++){
        for(int j = i; j >0; j--){
            if(arr[j] < arr[j - 1]){
                swap(arr[j], arr[j - 1]);
            }
            else{
                break;
            }
        }
    }
}