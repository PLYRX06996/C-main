#include<bits/stdc++.h>
using namespace std;
class Solution {
public:
    bool isPalindrome(string s) {
        string truncStr;
        truncStr.reserve(s.size());
        for(unsigned char c : s){
            int ascii = (int)c;
            if(ascii >= 97 && ascii <= 122 || ascii >= 48 && ascii <= 57){
                truncStr.push_back(char(ascii));
            } else if(ascii >= 65 && ascii <= 90){
                truncStr.push_back(char(ascii) + 32);
            }
        }
        string revStr(truncStr.rbegin(), truncStr.rend());
        return truncStr == revStr;
    }
};