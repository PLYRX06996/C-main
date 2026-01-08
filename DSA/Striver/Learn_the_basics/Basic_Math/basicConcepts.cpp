#include<bits/stdc++.h>
using namespace std;

/*PLaying eith digits.*/
/*Palindrome: palindroem are the number for which 
reverse is same as the original... all negative numbers are not palindrome*/

/*Armstrong number: if adding the cube of the digits makes the number it self then we call it as...*/
int reverseNUM(int x){
    if(x < 0) return 0;
    int rev = 0;
    while(x > 0){
        rev = rev * 10 + x % 10;
        x = x / 10;
    }
    return rev;
}

bool checkpalindrome(int original){
    int rev = reverseNUM(original);
    cout << rev << endl;
    return original == rev;
}

int calcNumOfDig(int num){
    int digits = 0;
    while(num > 0){
        digits++;
        num /= 10;
    }
    return digits;
}

int calcArmstrong(int num){
    if(num < 0) return 0;
    int NOD = calcNumOfDig(num);
    int sum = 0;
    while(num > 0){
        int temp;
        temp = num % 10;
        sum += pow(temp, NOD);
        num /= 10;
    }
    return sum;
}

bool checkArmstrong(int x){
    int sum = calcArmstrong(x);
    cout << sum << endl;
    return sum == x;
}

// void printAllDivisors(int n){
//     if(n < 0) {
//         cout << "bad option to check for divisors";
//         return;
//     } else {
//         int i = 1;
//         while(i < n){
//             if(n % i == 0) cout << i << " ";
//             i++;
//         }
//     }
// }
void printAllDivisors(int n){
    if(n < 0) {
        cout << "bad option to check for divisors";
        return;
    } else {
        vector<int> ls;
        int i = 1;
        while(i*i < n){
            if(n % i == 0) {
                ls.push_back(i);
            if(n/i != i){
                ls.push_back(n/i);
            } 
        }
            i++;
        }
        sort(ls.begin(), ls.end());
        for(auto p : ls) cout << p << " ";
    }
}

void checkPrime(int n){
    if(n <= 1) {
        cout << n << " is not a prime number";
        return;
    }
    int i = 2;
    bool isPrime = true;
    while(i*i <= n){
        if(n%i == 0){
            isPrime = false;
            if(n/i != i) isPrime = false;
            break;
        }
        i++;
    }
    if(isPrime) cout << n << " is a prime number" << endl;
    else cout << n << " is not a prime number" << endl;
}

int GCD(int n1, int n2){
    int gcd = 1;
    for(int i = min(n1, n2); i > 0; i--){
        if(n1%i ==0 && n2%i == 0){
            gcd = i;
            break;
        }
    }
    return gcd;
}

/*Eucledian algorithm for finding GCD: GCD(a, b) if a > b; GCD(a - b, b)*/
int GCDEuc(int x, int y){
    int m = min(x, y);
    int M = max(x, y);
    if(m <= 0) return 1;
    return GCD(M-m, m); //or GCD(M%m, m)
} //TC: O(log(phi)(min(a,b)))
int main() {
    // int n;
    // cin >> n;
    // if(checkpalindrome(n)) {
    //     cout << "the entered number is palindrome.";
    // } else cout << "the entered number is not a palindrome.";
    // if(checkArmstrong(n)) {
    //     cout << "the entered number is Armstrong number.";
    // } else cout << "the entered number is not an Armstrong number.";
    // printAllDivisors(n);
    // checkPrime(n);
    int n1, n2;
    cin >> n1 >> n2;
    cout << GCD(n1, n2) << endl;
    cout << GCDEuc(n1, n2);
    return 0;
}