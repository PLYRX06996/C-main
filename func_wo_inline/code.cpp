#include<iostream>
using namespace std;

inline int Maximum(int* x, int* y){
    int m = (*x>*y) ? *x : *y;
    return m;
}

int main(){

    int a, b;
    cin>>a>>b;
    cout<<"The maximum of a & b:"<<Maximum(&a,&b)<<endl;
    return 0;
}
