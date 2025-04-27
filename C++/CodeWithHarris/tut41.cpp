/*----------------------- MULTIPLE INHERITANCE---------------------------*/
// Syntax--->
// class derived : visibility-mode base-class1, visibility-mode base-class2 {};
#include<iostream>
#include<cmath>
using namespace std;

class Base1 {
    protected:
    int base1Var;
    public:
    Base1(int a) : base1Var(a) {}
};

int main() {

    return 0;
}