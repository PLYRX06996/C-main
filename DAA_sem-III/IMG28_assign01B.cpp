#include <iostream>
#include <vector>
using namespace std;

int findPeak(const vector<int>& arr) {
    int low = 0, high = arr.size() - 1;
    while (low < high) {
        int mid = (low + high) / 2;
        if (arr[mid] < arr[mid + 1])
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

int binarySearchIncreasing(const vector<int>& arr, int low, int high, int target) {
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int binarySearchDecreasing(const vector<int>& arr, int low, int high, int target) {
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == target) return mid;
        else if (arr[mid] > target) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int searchInBitonic(const vector<int>& prices, int target) {
    int peak = findPeak(prices);

    int index1 = binarySearchIncreasing(prices, 0, peak, target);
    int index2 = binarySearchDecreasing(prices, peak + 1, prices.size() - 1, target);

    if (index1 == -1 && index2 == -1) return -1;
    return max(index1, index2);
}

int main() {
    vector<int> prices = {90, 120, 150, 170, 160, 140, 150, 100};
    int target = 150;
    int result = searchInBitonic(prices, target);
    cout << "Last occurrence of " << target << " is at index: " << result << endl;
    return 0;
}
