// Static Data Members & Methods in C++ OOPS
#include <iostream>
using namespace std;
class Employee
{
private:
    int id;
    static int count; // class variable and property of the class

public:
    void setData()
    {
        cout << "Enter the id of employee: ";
        cin >> id;
        count++;
    }
    void getData()
    {
        cout << "The id of " << count << "th employee is " << id << endl;
    }
    // Static member functions can only access only static variables
    static void getCount(void)
    {
        cout << "The value of count is: " << count << endl;
    }
};
int Employee ::count; // count = n;
// dothis if using static
int main()
{
    Employee Abdul, Moin, Bill;
    Abdul.setData();
    Abdul.getData();
    Employee::getCount();

    Moin.setData();
    Moin.getData();
    Employee::getCount();

    Bill.setData();
    Bill.getData();
    Employee::getCount();
    return 0;
}