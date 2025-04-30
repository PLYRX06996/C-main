#include<iostream>
#include<cmath>
using namespace std;
/*------------- "new" and "delete" keywords in C++ -----------*/
int main() {
    int x1 = 10;
    int* ptr1 = &x1;
    cout << "The value at address ptr1 is: " << *ptr1 << endl;
    // <variable-type>* pointer-variable-name = new <variable-type>(variable-value);
    int *ptr2 = new int(15);
    cout << "The value at address ptr1 is: " << *ptr2 << endl;
    delete ptr2;
    int* ptr3 = new int[5]();
    for(int i = 0; i < 5; i++) {
        cout << "The value at arr[" << i << "] is: " << *(ptr3+i) << endl;
    }
    delete[] ptr3;
    return 0;
}
// 49 --> use of basic opinter
// 50--> use of ptr-> instead of *(ptr)
// 51 --> use of opinter as a class object type
// <class-name>* pointer-name = new <cllass-name>[] creating array of objects of a class if needed
// 52 --> properties of pointer