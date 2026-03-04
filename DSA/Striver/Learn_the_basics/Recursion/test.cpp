#include <bits/stdc++.h>
using namespace std;

string makeStrforCheck(const string& s)
{
    string S;
    for (unsigned char c : s)
    {
        if (isalnum(c))
        {
            S.push_back(tolower(c));
        }
    }
    return S;
}

bool isPalindrome(const string& s, int l, int r)
{
    if (l >= r) return true;
    if (s[l] != s[r]) return false;
    return isPalindrome(s, l + 1, r - 1);
}

int main()
{
    string s;
    getline(cin, s);

    s = makeStrforCheck(s);

    cout << "[" << s << "]\n";
    cout << isPalindrome(s, 0, s.size() - 1) << endl;
}