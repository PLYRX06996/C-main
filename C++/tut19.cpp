#include<iostream>
using namespace std;
// Function overloading
int product(int a, int b){ return a*b; }
int product(int a, int b, int c){ return a*b*c; }
int main(){
    int n1, n2, n3;
    cin >> n1 >> n2 >> n3;
    cout << "The product of first two given numbers: " << product(n1, n2) << endl;
    cout << "The product of first three given numbers: " << product(n1, n2, n3) << endl;
    return 0;
}