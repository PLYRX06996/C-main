#include<iostream>
#include<math.h>

using namespace std;

class triangle{
    private:
    int l1, l2, l3;
    public:
    void setlength(int a, int b, int c){
        l1 = a;
        l2 = b;
        l3 = c;
    }
    bool validityCheck(){
        if(l1 + l2 > l3 && l2 + l3 > l1 && l1 + l3 > l2){
            return true; 
            } else return false;
    }
    float calculateArea(int a, int b, int c){
        if(validityCheck()){
        float area;
        float s = (float)(a + b + c) / 2;
           area = sqrt(s*(s-a)*(s-b)*(s-c));
           return area;
        } else  cout << "Invalid triangle" << endl;
        
    }
    string typeOFtriangle(int a, int b, int c){
        if(validityCheck()){
        if (a == b && b == c){
            return "Equilateral Triangle";
            }
            else if (a == b || b == c || a == c){
                return "Isosceles Triangle";
                }
                else{
                    return "Scalene Triangle";
                    }
                    } else return "Invalid triangle";
    }
    void display(){
        float A = calculateArea(l1, l2, l3);
        cout << "The area of the trianle of given lenght is: "<<A<<endl;
        cout << "The type of the triangle is: "<<typeOFtriangle(l1, l2, l3);
    }
} t;

int main() {

    int l1, l2, l3;
    cin >> l1 >> l2 >> l3;
    t.setlength(l1, l2, l3);
    t.display();

    return 0;
}