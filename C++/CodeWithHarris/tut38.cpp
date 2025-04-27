#include<iostream>
using namespace std;
class Base {
    int data1;
    public:
    int data2;
    void setData();
    int getData1();
    int getdata2();
};
void Base ::setData(void) {
    data1 = 10; data2 = 20;
}
int Base ::getData1() {
    return data1;
}
int Base ::getdata2() {
    return data2;
}
class Derived : public Base {
    int data3;
    public:
    void process();
    void display();
};
void Derived ::process() {
    data3 = getData1() * getdata2();
}
void Derived ::display() {
    cout << "value of data-01 is:" << getData1() << endl;
    cout << "value of data-02 is:" << data2 << endl;
    cout << "value of data-03 is:" << data3 << endl;
}
int main() {
    Derived d1;
    d1.setData();
    d1.process();
    d1.display();
}