#include<iostream>
#include<iomanip>
using namespace std;

int main(){
    /*Constants in C++*/
    int x = 20;
    cout<<"Value of x was: "<<x<<endl;
    x = 30;
    cout<<"Value of x is: "<<x<<endl;
    const int y = 10; // Constant variable **Read only variable
    cout << "Value of y is: "<<y<<endl;
    //y = 20;  Error: cannot assign a value to a constant variable

    /*Manipulators in C++*/
    //They controls the display of data on console
    //(i)emdl (ii)setw()--> it hepls in fixing the feild width
    int a = 9, b = 99, c = 999, d = 9999;
    cout<<a<<endl<<b<<endl<<c<<endl<<d<<endl;
    cout<<setw(3)<<a<<endl<<setw(3)<<b<<endl<<setw(3)<<c<<endl<<setw(3)<<d<<endl;

    /*Operator precedence and Associativity Web: 
    "en.cppreference.com/w/cpp/language/operator_precedence"*/
    return 0;
}