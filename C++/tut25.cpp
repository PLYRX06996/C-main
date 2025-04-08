// Array of Objects
#include <iostream>
using namespace std;
class employee
{
    int id;
    float salary;

public:
    void setData()
    {
        cout << "Enter the employee Id: ";
        cin >> id;
        cout << "Enter the salary: ";
        cin >> salary;
    }
    void getData()
    {
        cout << "The salary of the employee " << id << " is " << salary << endl;
    }
};
int main()
{
    int nemp;
    cin >> nemp;
    employee E[nemp];
    for (int i = 0; i < nemp; i++)
    {
        E[i].setData();
    }
    for (int i = 0; i < nemp; i++)
    {
        E[i].getData();
    }

    return 0;
}