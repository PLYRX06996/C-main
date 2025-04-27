#include<iostream>
#include<math.h>
#include<limits.h>

using namespace std;

class factorial {
    private:
    int num;
    public:
    int getdata(){
        cout << "Enter an integer number to calculate it's factorial: ";
        cin >> num;
        
    }
    bool validityCheck(){
        if(num < 0){
            return false; 
            } else return true;
    }
    int factorialCalc(int n){
        if(validityCheck()){
        if (n == 0) return 1;
        else return (n * factorialCalc(n - 1));
        } else {
            return INT_MIN;
        }
    }
    void displayFact(){
        cout << "The factorial of "<< num << " is "<< factorialCalc(num) << endl;
    }
} num;

int main() {

    num.getdata();
    num.displayFact();      
    
    return 0;
}