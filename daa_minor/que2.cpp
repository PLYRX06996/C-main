#include <bits/stdc++.h>
using namespace std;

int medianOfThree(int arr[], int l, int h) {
    int mid = l + (h - l) / 2;
    int a = arr[l], b = arr[mid], c = arr[h];
    if ((a > b) != (a > c)) return l;
    else if ((b > a) != (b > c)) return mid;
    else return h;
}

void partitionArray(int arr[], int l, int h, int &p1, int &p2) {
    if (h - l < 1) {
        p1 = l;
        p2 = h;
        return;
    }
    int medianIdx = medianOfThree(arr, l, h);
    swap(arr[medianIdx], arr[h]);

    int pivot1 = arr[l];
    int pivot2 = arr[h];

    if (pivot1 > pivot2) {
        swap(arr[l], arr[h]);
        swap(pivot1, pivot2);
    }

    int i = l + 1, lt = l + 1, gt = h - 1;
    while (i <= gt) {
        if (arr[i] < pivot1) {
            swap(arr[i], arr[lt]);
            lt++;
        } else if (arr[i] > pivot2) {
            swap(arr[i], arr[gt]);
            gt--;
            i--;
        }
        i++;
    }
    lt--;
    gt++;
    swap(arr[l], arr[lt]);
    swap(arr[h], arr[gt]);
    p1 = lt;
    p2 = gt;
}

void quickSort(int arr[], int l, int h) {
    if (l < h) {
        int p1, p2;
        partitionArray(arr, l, h, p1, p2);
        quickSort(arr, l, p1 - 1);
        quickSort(arr, p1 + 1, p2 - 1);
        quickSort(arr, p2 + 1, h);
    }
}

int main() {
    int n;
    cout << "Enter the number of elements for the Array: ";
    cin >> n;

    int arr[n];
    cout << "Enter the elements of the Array: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    cout << "The Array before sorting is: ";
    for (auto i : arr) {
        cout << i << " ";
    }
    cout << endl;

    quickSort(arr, 0, n - 1);

    cout << "The Array after sorting is: ";
    for (auto i : arr) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}
