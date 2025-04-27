// Freind Functions
#include<iostream>
using namespace std;
class complex
{
private:
    int a;
    int b;
     // Below line means that non member - sumComplex funtion is allowed to do anything with my private parts (members)
    friend complex sumOfcomplex(complex o1, complex o2);
public:
    void setComplexNum(int a1, int b1)
    {
        a = a1;
        b = b1;
    }
    // void addComplexNum(complex n1, complex n2)
    // {
    //     a = n1.a + n2.a;
    //     b = n1.b + n2.b;
    // }
    void printCNum()
    {
        cout << "Your complex number is " << a << "+" << b << "i" << endl;
    }
};
complex sumOfcomplex(complex o1, complex o2){
    complex o3;
    o3.setComplexNum((o1.a + o2.a), (o1.b + o2.b));
    return o3;
}
int main(){
    complex c1, c2, sum;
    c1.setComplexNum(2, 5);
    c2.setComplexNum(7, 4);
    c1.printCNum();
    c2.printCNum();
    sum = sumOfcomplex(c1, c2);
    sum.printCNum();
    return 0;
}
/* Properties of friend functions
1. Not in the scope of class
2. since it is not in the scope of the class, it cannot be called from the object of that class. c1.sumComplex() == Invalid
3. Can be invoked without the help of any object
4. Usually contans the objects as arguments
5. Can be declared inside public or private section of the class
6. It cannot access the members directly by their names and need object_name.member_name to access any member.

*/