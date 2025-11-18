#include <iostream>
#include <vector>
using namespace std;

pair<int,int> partition(vector<int>& arr, int low, int high) {
    if (arr[low] > arr[low + 1]) {
        swap(arr[low], arr[low + 1]);
    }
    int p1 = arr[low], p2 = arr[low + 1];
    int lt = low + 2;
    int i  = low + 2;
    int gt = high;

    while (i <= gt) {
        if (arr[i] < p1) {
            swap(arr[i], arr[lt]);
            lt++;
            i++;
        } else if (arr[i] > p2) {
            swap(arr[i], arr[gt]);
            gt--;
        } else {
            i++;
        }
    }

    lt--;
    gt++;
    swap(arr[low + 1], arr[lt]);
    swap(arr[low], arr[lt - 1]);

    return {lt - 1, lt};
}

void ternaryPartitionSort(vector<int>& arr, int low, int high) {
    if (high - low <= 1) {
        if (low < high && arr[low] > arr[high]) {
            swap(arr[low], arr[high]);
        }
        return;
    }

    pair<int,int> pivots = partition(arr, low, high);
    int p1Index = pivots.first;
    int p2Index = pivots.second;

    ternaryPartitionSort(arr, low, p1Index - 1);
    ternaryPartitionSort(arr, p1Index + 1, p2Index - 1);
    ternaryPartitionSort(arr, p2Index + 1, high);
}

void ternaryPartitionSort(vector<int>& arr) {
    if (!arr.empty()) {
        ternaryPartitionSort(arr, 0, arr.size() - 1);
    }
}

int main() {
    int n;
    cin >> n;
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    ternaryPartitionSort(arr);
    cout<<"Sorted Array: ";
    for (int x : arr) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}