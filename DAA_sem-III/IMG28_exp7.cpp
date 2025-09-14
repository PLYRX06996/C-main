#include <bits/stdc++.h>
using namespace std;

void swap2(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void maxHeapify(int A[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && A[left] > A[largest])
        largest = left;
    if (right < n && A[right] > A[largest])
        largest = right;

    if (largest != i) {
        swap2(A[i], A[largest]);
        maxHeapify(A, n, largest);
    }
}

void printArray(int A[], int n) {
    for (int i = 0; i < n; i++)
        cout << A[i] << " ";
    cout << endl;
}

int main() {
    int A[] = {16, 4, 10, 8, 7, 9, 3, 2, 14, 1};
    int n = sizeof(A) / sizeof(A[0]);

    cout << "Before MAX-HEAPIFY:" << endl;
    printArray(A, n);

    maxHeapify(A, n, 1);

    cout << "After MAX-HEAPIFY:" << endl;
    printArray(A, n);

    return 0;
}