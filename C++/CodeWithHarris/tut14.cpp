#include<iostream>
using namespace std;

struct employee
{
    int EmpID;
    string Name;
    float salary;
};
/*OR 
typedef struct employee
{
    int EmpID;
    string Name;
    float slary;
}emp;*/

//Memory optimization using unions
union money
{
    int rice;
    float pounds;
};

int main(){
    /*--- Structure, Unions & Enums in C++ 
    These are user defined data types---*/
    employee Harry;
    //emp Harry;
    Harry.EmpID = 1;
    Harry.Name = "Harry Potter";
    Harry.salary = 50000.0;
    cout << "Employee ID: " << Harry.EmpID << endl;
    cout << "Employee Name: " << Harry.Name << endl;
    cout << "Employee Salary: " << Harry.salary << endl;

    union money m1;
    m1.rice = 50;
    //m1.pounds = 100.0; Error!
    cout << "Rice: " << m1.rice << endl;
    
    enum meal{breakfast, lunch, dinner};
    cout << "Meal_1: " << breakfast << endl; //0
    cout<< "Meal-2: " << lunch << endl; //1
    cout<< "Meal-3: " << dinner << endl; //2
    return 0;
}