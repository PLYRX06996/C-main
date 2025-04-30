#include<iostream>
#include<cmath>
using namespace std;
// Pointers to derived class
class Base{
    public:
    int baseVar;
    void display() {
        cout << "The value of Base class var is: " << baseVar << endl;
    }
};
class Derived : public Base {
    public:
    int derVar;
    void display() {
        cout << "The value of derived class var is: " << derVar << endl;
    }
};
int main() {
    Base* Baseptr = new Base;
    Base baseObj;
    Derived derObj;
    Baseptr = &derObj; // LATE BINDING
    baseObj.baseVar = 27;
    baseObj.display();
    Derived* Derptr = &derObj; // u can do the same things as you can do in derObj
    return 0;
}
