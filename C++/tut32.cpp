#include <iostream>
using namespace std;
class simple
{
    int data1;
    int data2;

public:
    simple(int a, int b = 9)
    {
        data1 = a;
        data2 = b;
    }
    void printData()
    {
        cout << "The values of data is: " << data1 << " and " << data2 << endl;
    }
};
int main()
{
    simple s1(1, 4), s2(5);
    s1.printData();
    s2.printData();
    return 0;
}