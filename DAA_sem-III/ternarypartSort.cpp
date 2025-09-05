#include <bits/stdc++.h>
using namespace std;

void swapVals(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void TernaryPartition(vector<int> &arr, int low, int high, int *m1, int *m2) {
    int p1 = arr[low];
    int p2 = arr[low + 1];

    if (p1 > p2) swapVals(p1, p2);

    int l = low + 2;
    int h = high;
    int i = l;

    while (i <= h) {
        if (arr[i] < p1) {
            swapVals(arr[i], arr[l]);
            l++;
            i++;
        }
        else if (arr[i] > p2) {
            swapVals(arr[i], arr[h]);
            h--;
        }
        else {
            i++;
        }
    }

    l--;
    h++;

    arr[low] = arr[l];
    arr[l] = p1;

    arr[low + 1] = arr[h - 1];
    arr[h - 1] = p2;

    *m1 = l;
    *m2 = h - 1;
}

void TernaryPartitionSort(vector<int> &arr, int l, int h) {
    if (l >= h) return;

    if (h - l == 1) {
        if (arr[l] > arr[h]) swapVals(arr[l], arr[h]);
        return;
    }

    int m1, m2;
    TernaryPartition(arr, l, h, &m1, &m2);

    TernaryPartitionSort(arr, l, m1 - 1);
    TernaryPartitionSort(arr, m1 + 1, m2 - 1);
    TernaryPartitionSort(arr, m2 + 1, h);
}

int main() {
    int n;
    cin >> n;
    vector<int> arr(n);

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    TernaryPartitionSort(arr, 0, n - 1);

    cout << "Sorted array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    return 0;
}
