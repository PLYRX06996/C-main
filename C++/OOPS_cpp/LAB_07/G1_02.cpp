#include<iostream>
#include<sstream>
using namespace std;
class rectangle{
    private:
    double length;
    double breadth;
    public:
    rectangle();
    rectangle(double , double);
    rectangle(double);
   void displayArea();
   ~rectangle(){}
};
rectangle::rectangle(){
    this->length = 0;
    this->breadth = 0;
}
rectangle::rectangle(double l, double b){
    this->length = l;
    this->breadth = b;
}
rectangle::rectangle(double d){
    this->breadth = this->length = d;
}
void rectangle::displayArea(){
    cout << "The area of the given rectangle is : " << length*breadth <<  endl;
}
double GetDoubleInput(string str){
    string input;
    double value = 0;
    cout << str;
    getline(cin, input);
    if(!input.empty()){
        stringstream ss(input);
        ss >> value;
        if(ss.fail()){
            value = INT16_MAX;
        }
    }
    return value;
}
int main() {
    int n;
    cout << "Enter the number of rectangles : ";
    cin >> n;
    cin.ignore();
    rectangle r[n];
    for (int i = 0; i < n; i++) {
        cout << "Rectangle " << (i + 1) << ":" << endl;
        double l = GetDoubleInput("Enter the length : ");
        double b = GetDoubleInput("Enter the breadth : ");
        if (l != 0 && b != 0) {
            r[i] = rectangle(l, b);  
        } else if (l == 0 && b != 0) {
            r[i] = rectangle(b);  
        } else if (l != 0 && b == 0){
            r[i] = rectangle(l);
        }
    }
    cout << "\nDisplaying Areas:\n";
    for (int i = 0; i < n; i++) {
        cout << "Rectangle " << (i + 1) << ":" << endl;
        r[i].displayArea();
    }
    return 0;
}