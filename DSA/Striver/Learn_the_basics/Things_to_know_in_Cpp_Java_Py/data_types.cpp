#include<bits/stdc++.h>
using namespace std;
int main() {
    /*there are couple of data types such as
    int
    double
    float
    long
    string
    char
    */
   cout << "Enter a string: ";
   string s;
   getline(cin, s); //in order to get the complete line as one string, you have ot use getline()
   // else each word separated by space will be considered as different string.
   cout << "Your Entered String is: " << "'" << s << "'" << endl; 
   return 0;
}