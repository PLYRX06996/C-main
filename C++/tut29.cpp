/*      CONSTRUCTORS & DESTRUCTORS      */
#include<iostream>
using namespace std;
class Complex{
    int a, b;
    public:
    //constructor is a special meber function with same name
    // as of the class, It is automatically invoked whenever 
    // an object is created
    //it is used to initialize the object of its class
    // It should be declared in public section of class
    // They cannot return values and do not have return types
    // It can have default arguments
    // We cannot refer to their address
    Complex();
    void printCNum()
    {
        cout << "Your complex number is " << a << "+" << b << "i" << endl;
    }
};
Complex::Complex(){ // This is a default constructor
    a = 10;         // coz it does not take any arguments
    b = 5;
}
int main(){
    Complex c1, c2, c3;
    c1.printCNum();
    c2.printCNum();
    c3.printCNum();
    return 0;
}