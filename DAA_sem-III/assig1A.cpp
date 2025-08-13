/*Question:
Over a span of n days, a stock followed a unique price trend. You’re
investigating the stock chart to find when a specific price occurred.
The stock prices over n days are stored in an array.
Your task is to find the last day (maximum index) when the stock had
a given target price. If the element doesn’t exist, return -1.

Part 1:
The stock prices may have duplicate values anywhere.

Example:
prices = [100, 120, 120, 130, 130, 130, 125, 110, 90]
target = 130
Output: 5*/
#include<iostream>
using namespace std;
void Iarray(int arr[], int s){
    cout << "Enter the elements one-by-one separating each by a space.:" <<  endl;
    for(int i = 0; i < s; i++){
        cin >> arr[i];
    }
}
void LinearSearch(int arr[], int s){}
int main(){
    int size;
    cout << "Enter the array size: ";
    cin >> size;
    int arr[size];
    Iarray(arr, size);
    return 0;
}