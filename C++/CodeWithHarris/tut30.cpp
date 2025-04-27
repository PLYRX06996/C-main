#include<iostream>
using namespace std;
class Complex{
    int a, b;
    public:
    Complex(int , int);
    void printCNum()
    {
        cout << "Your complex number is " << a << "+" << b << "i" << endl;
    }
};
Complex::Complex(int x, int y){ // This is a default constructor
    a = x;       // coz it does not take any arguments
    b = y;
}
int main(){
    //Implicite call
    Complex c1(4, 5);
    c1.printCNum();
    //Explicite call
    Complex c2 = Complex(5, 7);
    c2.printCNum();
    return 0;
}