#include <iostream>
using namespace std;
// Forward declaration
class complex;
class calculator
{
public:
    int sum(int a, int b)
    {
        return a + b;
    }
    int sumRealComplex(complex o1, complex o2);
    int sumCompComplex(complex o1, complex o2);
};
class complex
{
private:
    int a;
    int b;
    //Individuallyy declaring freind functions
    // friend int calculator::sumRealComplex(complex o1, complex o2);
    // friend int calculator::sumCompComplex(complex o1, complex o2);
    friend class calculator;
public:
    void setComplexNum(int a1, int b1)
    {
        a = a1;
        b = b1;
    }
    void printCNum()
    {
        cout << "Your complex number is " << a << "+" << b << "i" << endl;
    }
};
int calculator::sumRealComplex(complex o1, complex o2)
{
    return (o1.a + o2.a);
}
int calculator::sumCompComplex(complex o1, complex o2){
    return (o1.b + o2.b);
}
int main()
{
    complex o1, o2;
    o1.setComplexNum(1, 4);
    o2.setComplexNum(5, 7);
    calculator calc;
    int resR = calc.sumRealComplex(o1, o2);
    cout << "The sum of real parts of o1 and o2 is: " << resR << endl;
    int resC = calc.sumCompComplex(o1, o2);
    cout << "The sum of complex parts of o1 and o2 is: " << resC << endl;
    return 0;
}