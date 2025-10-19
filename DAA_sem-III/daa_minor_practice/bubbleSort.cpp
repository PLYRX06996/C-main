#include<bits/stdc++.h>
using namespace std;
int main() {
    
    return 0;
}
void bubbleSort(vector<int>& arr){
        int didSwap = 0;
        for(int i= 0; i < arr.size() - 1; i++){
            for(int j = 0; j < arr.size() - i - 1; j++){
                if(arr[j] > arr[j + 1]){
                    swap(arr[j], arr[j + 1]);
                    didSwap = 1;
                }
            }
            if(didSwap == 0){
                break;
            }
        }
    }