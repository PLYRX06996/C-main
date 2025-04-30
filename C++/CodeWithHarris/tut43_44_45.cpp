#include<iostream>
#include<cmath>
using namespace std;
/*--------------- Ambiguity Resolution in Inheritance.-------------- 
Solution: 
Pro method:      virtual function override OR
Dumbass  method: use scope resolution operator
Agar kisi class mein vo name wala function hai aur tab object call lagta hai to
 usi class ka function execute hota hai, agar base class mein same func hai phir bhi THIS IS CALLED METHOD OVERRIDING
aur agar class mein vo function nhi hai to jis class se inherit kiya hai uska vo name wala function use hoga.*/

/*---------------- Virtual Base Class---------------------
Synrtax-->  class-derived : virtual <visibility-mode> base-class {};
this prevents duplication in multilevel Inheritance.*/
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
class sports : public Student {
    protected:
    float speed;
    float BMI;
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
    
    return 0;
}