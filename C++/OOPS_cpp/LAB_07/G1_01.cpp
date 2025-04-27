#include<iostream>
using namespace std;
class Student{
    private:
    string name;
    string rollNumber;
    public:
    static int count;
    Student(){
        string name, roll;
        cout << "Enter student name : ";
        getline(cin, name);
        cout << "Enter roll number : ";
        getline(cin, roll);
        this->name = (name.empty()) ? "Unknown" : name;
        this->rollNumber = (roll.empty()) ? "2024IMG-XYZ" : roll;
    }
    // Student(string name, string rollNumber) {
    //     this->name = (name.empty()) ? "Unknown" : name;
    //     this->rollNumber = (rollNumber.empty()) ? "2024IMG-XYZ" : rollNumber;
    // }
    void printName(){
        count++;
        cout << count << ":" << endl;
        cout << "Name : " << this->name << endl;
        cout << "Roll Number : " << this->rollNumber << endl;
    }
};
int Student::count = 0;
int main() {
    Student s1, s2, s3;
    s1.printName();
    s2.printName();
    s3.printName();
    return 0;
}