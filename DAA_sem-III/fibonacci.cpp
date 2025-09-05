#include<bits/stdc++.h>
#include<ctime>
using namespace std;

void printFibonacci(int n){
    int t1 = 0, t2 = 1;
    cout << t1 << " " << t2 << " ";
    
    for (int i = 0; i < n - 2; i++)
    {
        int tl = t2 + t1;
        cout << tl << " ";
        t1 = t2;
        t2 = tl;
    }
}

void Fibonaccihelper(int t1, int t2, int i, int n){
    if(i < n){
        int tl =  t1 + t2;
        cout << tl << " ";
        i++;
        t1 = t2;
        t2 = tl;
        Fibonaccihelper(t1, t2, i, n);
    }
    return;
}

void printFibonacciRec(int n){
    int t1 = 0, t2 = 1;
    cout << t1 << " " << t2 << " ";
    int i = 2;
    Fibonaccihelper(t1, t2, i, n);
}

int main() {
    int n;
    cin >> n;
    time_t s = time(nullptr);
    printFibonacci(n);
    time_t e = time(nullptr);
    cout << "Apparently, it's going to take you " << e - s << " seconds.\n";
    cout << endl;
    cout << "------- Printing Fibonacci Recursively---------" << endl;
    s = time(nullptr);
    printFibonacciRec(n);
    e = time(nullptr);
    cout << "Apparently, it's going to take you " << e - s << " seconds.\n";
    return 0;
}