#include<iostream>
#include<cmath>
using namespace std;

class Base{
    private:
    int a;
    protected:
    int b;
};
// for a protected member 
//                 public derivation | private derivation | protected derivation
// public              public              private             protected
// private          Not Inherited       Not Inherited       Not Inherited
// protected           protected           private             protected
class Derived : protected Base{

};

int main() {
    Base b1;
    Derived d1;
    // cout << b1.a << d1.a; will not work
    // cout << b1.b << d1.b;
    return 0;
}