/*          DESTRUCTOR          */
// it frees dynamically allocated memory created by constructor implecitly
// never takes an arguments nor does it returns any value
#include<iostream>
using namespace std;
int count  = 0;
class num{
    int n;
    public:
    num(){ 
        cout<<"The coonstructor has been called for creating object number"<< count++ <<endl;
    }
    ~num(){
        cout<<"The destructor has been called for destroying object number."<< count-- <<endl;
    }
};

int main(){
    cout << "Entering the main" << endl;
    cout << "Creating first object..." << endl;
    num n1;
    {
        cout << "Inside first block." << endl;
        cout << "Creating other two objects..." << endl;
        num n2, n3;
        cout << "Exiting the block." << endl;
    }
    cout << "Back to main." << endl;
    
    return 0;
}