/*--------------- How constructor works in Derived class-------------
1. The constructor for a virtual base class is invoked before a non-virtual base class
2. If there ar multiple base classes they are invoked in that order.
3. Any non-virtual base class constructor are then constructed before derived class constructor is executed.*/
#include<iostream>
#include<cmath>
using namespace std;
/*
Case 1:
class B : public A{
    order of execution of constructor: A() --> B()
};
Case 2:
class A: public B, public C{
    order of execution of constructors: B() --> C() --> A()
};
Case 3:
class A: public B, virtual public C{
    order of execution of constructors: C() --> B() --> A()
};
-----------------------------INiTIALIZATION IS DONE IN THE SAME ORDER AS DECLARED BEFORE----------------------------
*/
class Base1 {
    protected:
    int B1var1;
    public:
    Base1(int x) : B1var1(x) { cout << "The constructor for Base class 1 is called." << endl;}
    void displayBase1Vars() {cout << "Base1 var1: " << B1var1 << endl;}
};
class Base2 {
    protected:
    int B2var1;
    public:
    Base2(int x) : B2var1(x) { cout << "The constructor for Base class 2 is called." << endl;}
    void displayBase2Vars() {cout << "Base2 var1: " << B2var1 << endl;}
};
class Derived : public Base1, public Base2 {
    protected:
    int Dvar1, Dvar2;
    public:
    Derived(int w, int x, int y, int z) : Base1(w), Base2(x), Dvar1(y), Dvar2(z) { cout << "The constructor for derived class has been called." <<  endl;}
    void displayDerivedVars() {cout << "Derived vars: " << Dvar1 << " " << Dvar2 << endl;}
};
int main() {
    Derived d1(1, 2, 3, 4);
    d1.displayBase1Vars();
    d1.displayBase2Vars();
    d1.displayDerivedVars();
    return 0;
}