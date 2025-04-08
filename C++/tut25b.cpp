#include <iostream>
using namespace std;
class complex
{
private:
    int a;
    int b;

public:
    void setComplexNum(int a1, int b1)
    {
        a = a1;
        b = b1;
    }
    void addComplexNum(complex n1, complex n2)
    {
        a = n1.a + n2.a;
        b = n1.b + n2.b;
    }
    void printCNum()
    {
        cout << "Your complex number is " << a << "+" << b << "i" << endl;
    }
};
int main()
{
    complex c1, c2, c3;
    c1.setComplexNum(1, 2);
    c1.printCNum();
    c2.setComplexNum(2, 3);
    c2.printCNum();
    c3.addComplexNum(c1, c2);
    c3.printCNum();
    return 0;
}