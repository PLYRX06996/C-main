/*  CONSTRUCTOR OVERLOADING */
#include<iostream>
using namespace std;
class Complex{
    int a, b;
    public:
    Complex(int , int);
    Complex(int );
    Complex();
    void printCNum()
    {
        cout << "Your complex number is " << a << "+" << b << "i" << endl;
    }
};
Complex::Complex(int x, int y){ 
    b = y;
}
Complex::Complex(int x){
    a = x;
    b = 0;
}
Complex::Complex(){
    a = 0;
    b = 0;
}
int main(){
    Complex c1(4, 5), c2(7), c3;
    c1.printCNum();
    c2.printCNum();
    c3.printCNum();
    return 0;
}