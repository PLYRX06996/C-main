#include<iostream>
using namespace std;

int c = 10;
int main(){
    /*Variable Scope & Scope resolution operator.*/
    int a,b,c;
    cin>>a>>b;
    c = a + b;
    cout<<"The local c is: "<<c<<endl;
    cout<<"The global c is: "<<::c<<endl;

    /*Literals - float, double, long double*/
    //By default decimal number is always considered as a 'double' ex.:- 34.4
    //34.4f or 34.4F is considered as a float
    //34.4l or 34.4L is considered as a long double
    float d = 34.4;
    long double e = 34.4;
    cout<<"The value of d is: "<<d<<endl;
    cout<<"The value of e is: "<<e<<endl;
    cout<<"The sizeof(34.4): "<<sizeof(34.4)<<endl;//double
    cout<<"The sizeof(34.4f): "<<sizeof(34.4f)<<endl;//float
    cout<<"The sizeof(34.4l): "<<sizeof(34.4l)<<endl;//long double
    cout<<"The sizeof(34.4F): "<<sizeof(34.4F)<<endl;//float
    cout<<"The sizeof(34.4L): "<<sizeof(34.4L)<<endl;//long double

    /*--------------Reference Veriable----------------*/
    float x = 3.1415;
    float &y = x;//y is a reference variable of x
    cout<<"The value of x is: "<<x<<endl;
    cout<<"The value of y is: "<<y<<endl;

    /*-----------Type Casting------------*/
    int p=69;
    cout<<p<<endl;
    float q = p;//Implicit type casting
    cout<<"The value of q is: "<<q<<endl;
    float r=2.718;
    cout<<r<<endl;
    int s = r;//Implicit type casting
    cout<<"The value of s is: "<<s<<endl;
    cout<<(int)r<<endl;
    cout<<int(r)<<endl;
    cout<<"The value of the expression p + r is: "<<p + r<<endl;
    cout<<"The value of the expression p + (int)r is: "<<p + (int)r<<endl;
    cout<<"The value of the expression p + int(r) is: "<<p + int(r)<<endl;
    cout<<"The value of the expression p + s is: "<<p + s<<endl;
    return 0;
}