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
class Base2 {
    protected:
    int base2Var;
    public:
    Base2(int a) : base2Var(a) {}
};
class derived : public Base1, public Base2 {
    protected:
    int derVar;
    public:
    void show() {
        cout << "The base-variable-01 is: " << base1Var <<  endl;
        cout << "The base-variable-02 is: " << base2Var <<  endl;
        cout << "The sum of base-variables is: " << base1Var + base2Var <<  endl;
    }
};
int main() {
    derived d1;
    

    return 0;
}