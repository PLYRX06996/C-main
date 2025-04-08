/*          DESTRUCTOR          */
// it frees dynamically allocated memory created by constructor implecitly
// never takes an arguments nor does it returns any value
#include<iostream>
using namespace std;

class num{
    int n;
    public:
    num(){ 
        cout<<"The coonstructor has been called."<<endl;
    }
    ~num(){
        cout<<"The destructor has been called."<<endl;
    }
};

int main(){
    
    return 0;
}