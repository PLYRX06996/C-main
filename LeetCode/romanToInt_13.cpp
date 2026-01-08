#include<bits/stdc++.h>
using namespace std;
class Solution {
public:
    int romanToInt(string s) {
        static const unordered_map<char,int> val = {
            {'I', 1}, {'V', 5}, {'X', 10}, {'L', 50},
            {'C', 100}, {'D', 500}, {'M', 1000}
        };
        
        int sum = 0;
        int prev = 0;

        int i = s.size() - 1;
        while (i >= 0) {
        int cur = val.at(s[i]);
        if (cur < prev) sum = sum - cur;
        else sum = sum + cur;
        prev = cur;
        i--;
        }
        return sum;
    }
};

class Solution {
public:
	int romanToInt(string s) {
		vector<int> str = {};
 		for (int i = 0;i < s.size();i++) {
			if (s[i] == 'I') str.push_back(1);
			else if (s[i] == 'V') str.push_back(5);
			else if (s[i] == 'X') str.push_back(10);
			else if (s[i] == 'L') str.push_back(50);
			else if (s[i] == 'C') str.push_back(100);
			else if (s[i] == 'D') str.push_back(500);
			else if (s[i] == 'M') str.push_back(1000);
		}
		int sum = 0;
        for (int i = 0; i < str.size() - 1; i++) {
            if (str[i] < str[i + 1])
                sum = sum - str[i];
            else
                sum = sum + str[i];
        }
        sum = sum + str[str.size() - 1];
		return sum;
	}
};