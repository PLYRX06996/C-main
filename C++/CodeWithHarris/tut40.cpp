#include<iostream>
#include<cmath>
using namespace std;
class Student {
    protected:
    int roll_num;
    public:
    void set_roll(int);
    void get_roll(void);
};
void Student::set_roll(int R) { roll_num = R;}
void Student::get_roll(){ cout << "The Roll Number is: " << roll_num << endl;}
class Exam : public Student {
    protected:
    double mathM;
    double langM;
    public:
    void set_marks(double, double);
    void get_marks(void);
};
void Exam::set_marks(double MM, double LM) { mathM = MM; langM = LM;}
void Exam::get_marks(){ cout << "The marks obtained in Math are: " << mathM << endl << "The marks obtained in Language are: " << langM << endl;}
class Result : public Exam {
    float percentage;
    public:
    void didplay() {
        get_roll();
        get_marks();
        cout << "Your percentage is: " << (mathM + langM) / 2 << endl;
    }
};
int main() {
    Result Abdul;
    Abdul.set_roll(420);
    Abdul.set_marks(92, 85);
    Abdul.didplay();
    return 0;
}
/*
Notes:
if we are inheriting B from A and C from B: [A-->B-->C]
1. A is the base class for B, and B is the base class for C.
2. A-->B-->C is called inheritance path*/