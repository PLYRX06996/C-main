#include<iostream>
#include<string>
using namespace std;

struct personal{
    private:
    string name;
    double salary;
    string DOJoining;
    public:
void setname(string n){
    name = n;
}
void setsalary(double s){
    salary = s;
}
void setDOJoining(string d){
    DOJoining = d;
}
string getname(){
    return name;
}
int getsalary(){
        return salary;
}
string getDOJoining(){
    return DOJoining;
}
void display(){
    cout<<"Name: "<<name<<endl;
    cout<<"Salary: "<<salary<<endl;
    cout<<"Date of Joining: "<<DOJoining<<endl;
}
    };

int main() {
    personal P1;
    cout<<"Enter your name:"<<endl;
    string n;
    getline(cin, n);
    P1.setname(n);
    cout<<"Enter your salary:"<<endl;
    double s;
    cin>>s;
    P1.setsalary(s);
    cin.ignore();
    cout<<"Enter your Date of Joining:"<<endl;
    string d;
    getline(cin, d);
    P1.setDOJoining(d);
    P1.display();
    return 0;
}