#include<iostream>
#include<cmath>
using namespace std;
/*-------------- Virtual Functions ---------------- */
class Base{
    public:
    int baseVar;
    virtual void display() {
        cout << "The value of Base class var is: " << baseVar << endl;
    }
};
class Derived : public Base {
    public:
    int derVar = 69;
    void display() {
        cout << "The value of derived class var is: " << derVar << endl;
    }
};
int main() {
    Base* Baseptr = new Base;
    Base baseObj;
    Derived derObj;
    Baseptr = &derObj; // LATE BINDING
    Baseptr->baseVar = 27;
    Baseptr->display();
    Derived* Derptr = &derObj; // u can do the same things as you can do in derObj
    derObj.derVar = 53;
    derObj.display();
    return 0;
}
