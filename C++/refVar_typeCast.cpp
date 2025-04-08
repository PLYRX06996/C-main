#include<iostream>
using namespace std;
int c = 5.95;
//scope resolution operator: '::'

int main(){
    // int a, b, c;
    // cin>>a>>b;
    // c = a + b;
    // cout<<c<<endl;//it will print local 'c'
    // cout<<::c;//it will print global 'c'
    float d=34.4;
    long double e=34.4;
    cout<<"The value of d is: "<<d<<endl<<"The value of e is: "<<e<<endl;
    return 0;
}