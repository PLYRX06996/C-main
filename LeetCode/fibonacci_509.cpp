#include<bits/stdc++.h>
using namespace std;

class Solution {
public:
    int fib(int n) {
        if(n == 0 || n == 1) return n;
        vector<int> arr(n+1);
        arr[0] = 0; arr[1] = 1;
        int i = 2;
        while(i <= n){
            arr[i] = arr[i - 1] + arr[i - 2];
            i++;
        }
        return arr[n];
    }
};