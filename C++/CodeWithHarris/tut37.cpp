#include<iostream>
using namespace std;
class Employee{
public: 
int id;
float salary;
Employee(int empID) : id(empID) , salary(34.0) {}
Employee() {}
void display() {
    cout << "ID:" << id << " Salary:" << salary << endl;
}
};
class Programmer : public Employee {
    int langCode;
    public:
    Programmer(int Eid, int Lid) : langCode(Lid) {
        id = Eid;
        salary = 100.0;
    }
    void getData() {
        cout << id << endl << langCode << endl << salary << endl;
    }

};
//syntax for derived class
/*
class derived-class-name : visibility-mode base-class-name {
The visibilty mode is how you wanted to inherit the derived class from base class? Public private??
1. Default visibility is Private.
2. Public Visibility: public members of base class becomes public members of derived class
3. Private: public members of the base class becomes private members of the derived class
4. You can never Inherit the Private members or member functions of base class to derived class
}

*/
int main() {
    cout << "main starts..." << endl;
    Employee e1(1) , e2 (2);
    e1.display();
    e2.display();
    Programmer p1(3, 7);
    p1.getData();
    return 0;
}
