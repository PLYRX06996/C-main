#include <iostream>
using namespace std;
class c2;
class c1
{
    int val1;

public:
    friend void exchange(c1 &x, c2 &y);
    void indata(int n)
    {
        val1 = n;
    }
    void display()
    {
        cout << val1 << endl;
    }
};
class c2
{
    int val2;

public:
    friend void exchange(c1 &x, c2 &y);
    void indata(int n)
    {
        val2 = n;
    }
    void display()
    {
        cout << val2 << endl;
    }
};
void exchange(c1 &x, c2 &y)
{ // reference to data of type c1 and type c2
    int temp = y.val2;
    y.val2 = x.val1;
    x.val1 = temp;
}
int main()
{
    c1 Oc1;
    c2 Oc2;
    Oc1.indata(34);
    Oc2.indata(26);
    exchange(Oc1, Oc2);
    cout << "value of data of c1 after exchange: ";
    Oc1.display();
    cout << "value of data of c2 after exchange: ";
    Oc2.display();
    return 0;
}