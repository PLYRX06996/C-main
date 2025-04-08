 #include<iostream>
 using namespace std;
 
 class employee{
    private:
    int a, b, c;
    public:
    int d, e;
    void setData(int a1, int b1, int c1); // Declaration
    void getData(){
        cout << "The value of a is: " << a << endl;
        cout << "The value of b is: " << b << endl;
        cout << "The value of c is: " << c << endl;
        cout << "The value of d is: " << d << endl;
        cout << "The value of e is: " << e << endl;
    }
 };
 // type class :: name (arguments)
 void employee :: setData(int a1, int b1, int c1){
        a = a1; b = b1; c = c1;
 }
 int main(){
    employee harry;
    harry.d = 13;
    harry.e = 17;
    // harry.a = 10; ---> Wrong!!! can't directly access a, b, c
    harry.setData(10, 20, 30);
    harry.getData();
    return 0;
 }