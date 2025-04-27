
/*                  COPY CONSTRUCTOR                */
//                  AAPKO HAMESHA DEFAULT CONSTRUCTOR BANANA PADTA HAI
// copy constructor doosre object ki copy bana deta hai
// agar class mein aapne explecitly provide nahi kiya hai copy constructor toh
// compiler automatically khud se copy constructor provide kar deta hai
#include<iostream>
using namespace std;

class Number{
    int a;
    public:
    Number() : a(0) {}
    Number(int val) : a(val) {}
    //creating copy construct
    Number(Number &obj){
        cout << "Copy constructor invoked." << endl;
        a = obj.a;
    }
   void display(){
        cout << "The value is: " << a << endl;
   }
};

int main(){
    Number x(10), y(43), z(27), z2;
    x.display();
    y.display();
    z.display();
    /*tab use karo jab copy construct nahi banaya
     Number z1 = Number(z); 
     Number z1(z);*/
     /*Agar copy construct banaya hai toh
        Number z1(z); isi ko use karo*/
    Number z1(z);
    z1.display();
    z2 = x; // copy constructor hasn't been called.
    z2.display();
    Number z3 = y; //copy constructor is called.
    z3.display();   
    return 0;
}