#include<iostream>
using namespace std;
/*  Inline functions, Default arguments, Constant arguments.  */

// Do not make every function inline
// Inline keyword changes the function call line into the function code
// if your function is too big then it is not advisable
// Lot's of cache memory will be unnecessarily consumed
// Note: When not to use inline strictly 
// 1. during recursion call func
// 2. static variables

float moneyReturn(int money, float factor = 1.04){ // Default argument works when we haven't gave 
    return money * factor;                        // value to that var in function call
}
// Constant arguments are ment to be not changable
int main(){
    // The most important thing I learned is 
    /*-------------- static type var = value; -----------------*/
    int money;
    cin >> money;
    cout << "If you had invested " << money << " rupees, then you would have got " << moneyReturn(money) << endl;
    return 0;
}