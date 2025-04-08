#include<iostream>
using namespace std;

class contstants{
    private:
    mutable int x;

    public:
    void display() const{
        cout<<"Value of x is "<<x<<endl;
        x++;
        cout<<"The value of x after increment is: "<<x<<endl;
    }
}konst;

int main(){
    konst.display();
    return 0;
}