#include<iostream>
using namespace std;
class Y;
class X{
    int data;
    public:
    void setValue(int value){
        data = value;
    }
    friend void add(X o1, Y o2);
};
class Y{
    int num;
    public:
    void setValue(int value){
        num = value;
    }
    friend void add(X o1, Y o2);
};
void add(X o1, Y o2){
    cout<<"Adding the data of X and Y object gives me: "<<o1.data + o2.num<<endl;
}
int main(){
    X x1;
    x1.setValue(5);
    Y y1;
    y1.setValue(6);
    add(x1, y1);
    return 0;
}