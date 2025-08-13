#include<bits/stdc++.h>
using namespace std;
int main() {
    /*Arrays And Strings*/
    int arr[5];
    //the memory allocation fo rarray data type is consecutive

    //2D arrays (imagine as a matrix)

    //the strings are like pre-built array of char (indexing is as same as array (0-n))
    //eg:
    char arr[7];
    arr[0] = 'S'; 
    arr[1] = 't'; 
    arr[2] = 'r'; 
    arr[3] = 'i'; 
    arr[4] = 'v'; 
    arr[5] = 'e'; 
    arr[6] = 'r';

    // is equivalent to 
    string s = "Striver";
    cout << s[0];
    // s[0]-->S 
    // s[1]-->t
    // s[2]-->r
    // s[3]-->i
    // s[4]-->v
    // s[5]-->e
    // s[6]-->r

    int len = s.length();
    // cout << len; ---> 7
    return 0;
}