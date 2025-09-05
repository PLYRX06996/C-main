#include <bits/stdc++.h>
using namespace std;

void swapVals(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void TernaryPartition(vector<int> &arr, int low, int high, int *m1, int *m2) {
    if (high - low < 1) return;
    
    // Choose first two elements as pivots
    int pivot1 = arr[low];
    int pivot2 = arr[low + 1];
    
    // Ensure pivot1 <= pivot2
    if (pivot1 > pivot2) {
        swapVals(pivot1, pivot2);
    }
    
    // Initialize pointers
    int smaller = low + 2;  // Points to next position for elements < pivot1
    int larger = high;      // Points to next position for elements > pivot2
    int current = low + 2;  // Current element being processed
    
    // Partition the array
    while (current <= larger) {
        if (arr[current] < pivot1) {
            swapVals(arr[current], arr[smaller]);
            smaller++;
            current++;
        }
        else if (arr[current] > pivot2) {
            swapVals(arr[current], arr[larger]);
            larger--;
            // Don't increment current as we need to check the swapped element
        }
        else {
            // Element is between pivot1 and pivot2, just move forward
            current++;
        }
    }
    
    // Place pivot1 in correct position
    smaller--;
    swapVals(arr[low], arr[smaller]);
    
    // Place pivot2 in correct position  
    larger++;
    swapVals(arr[low + 1], arr[larger]);
    
    // Return the positions of the pivots
    *m1 = smaller;
    *m2 = larger;
}

void TernaryPartitionSort(vector<int> &arr, int low, int high) {
    if (low >= high) return;
    
    // Base case: if only 2 elements, sort them directly
    if (high - low == 1) {
        if (arr[low] > arr[high]) {
            swapVals(arr[low], arr[high]);
        }
        return;
    }
    
    int m1, m2;
    TernaryPartition(arr, low, high, &m1, &m2);
    
    // Recursively sort the three partitions
    TernaryPartitionSort(arr, low, m1 - 1);        // Sort elements < pivot1
    TernaryPartitionSort(arr, m1 + 1, m2 - 1);     // Sort elements between pivots
    TernaryPartitionSort(arr, m2 + 1, high);       // Sort elements > pivot2
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;
    
    if (n < 2) {
        cout << "Need at least 2 elements for ternary partition sort." << endl;
        return 1;
    }
    
    vector<int> arr(n);
    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    cout << "Original array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;
    
    TernaryPartitionSort(arr, 0, n - 1);
    
    cout << "Sorted array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;
    
    return 0;
}