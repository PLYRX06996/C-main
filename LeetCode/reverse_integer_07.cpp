#include<bits/stdc++.h>
class Solution {
public:
    int reverse(int x) {
    long long num = x;
    bool check = false;
    if(num < 0){
        check = true;
        num = -1 * num;
    }
    long long reverse = 0;
    while(num > 0){
        long long temp = num % 10;
        reverse = reverse * 10 + temp;
        num = num / 10;
    }
    if(reverse > INT32_MAX) return 0;
    return check ? -1 * reverse : reverse;
    }
};