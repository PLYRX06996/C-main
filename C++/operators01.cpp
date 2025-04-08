#include<iostream>
using namespace std;

int main(){

        bool exp1=true;
        bool exp2=false;

    cout<<(exp1&&exp2)<<endl;
    cout<<(exp1||exp2)<<endl;
    cout<<(!exp1)<<endl;
    cout<<(!exp2)<<endl;

    //assignment operator
    int num1=6;
    num1+=3;
    int num2=3;
    num2-=2;
    cout<<num1<<endl<<num2<<endl;

    //bitwise operator

    int num3=5;
    cout<<(num3<<1)<<endl;
    cout<<(num3>>1)<<endl;

    int num4=8;

    cout<<(num3|num4)<<endl;
    cout<<(num3&num4)<<endl;

    //Miscellaneous operator

   int a=7;
    cout<<sizeof(a)<<endl;
    char name[]="Abdul Qadir";
    cout<<name<<endl;
    cout<<sizeof(name)<<endl;

    cout<<"locations:"<<endl<<&num1<<endl<<&num2<<endl<<&num3<<endl<<&num4<<endl<<&name<<&a<<endl<<&exp1<<endl<<exp2<<endl;

    //unary operators  

    int b=11;
    cout<<"Increment:"<<endl;
    cout<<b<<endl;
    cout<<(b++)<<endl;
    cout<<b<<endl;
    cout<<(++b)<<endl;
    cout<<b<<endl;

    int c=5;
    cout<<"Decrement:"<<endl;
    cout<<c<<endl;
    cout<<(c--)<<endl;
    cout<<c<<endl;
    cout<<(--c)<<endl;
    cout<<c<<endl;

    bool p=false;
    bool q=false;
    bool r=true;
    cout<<(p==q==r);
    return 0;
}