#include<iostream>
#include<iomanip>
using namespace std;

class house{
    private: 
    double length;
    double width;
    public:
    void setLenght(){
        double l;
        cout<<"Enter the lenght of the House: ";
        cin>>l;
        length = l;
    }
    void setwidth(){
        double w;
        cout<<"Enter the width of the House: ";
        cin>>w;
        width = w;
    }
    double getArea(){
        return length*width;
    }
    void display(){
        cout<<"The Area Of the house is "<<setprecision(4)<<getArea()<<endl;
    }
}H1;

int main() {
    H1.setLenght();
    H1.setwidth();
    H1.display();
    return 0;
}