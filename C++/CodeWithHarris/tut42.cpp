#include<iostream>
#include<cmath>
using namespace std;
#define PI 3.1415926535897932384626433832
class simpleCalc {
    protected:
    double Svar1, Svar2;
    public:
    void setvalues();
    void display();
};
void simpleCalc::setvalues() {
    cout << "Initializing Simple Calculator..." << endl;
    cout << "Enter the values for two operands a & b:";
    cin >> Svar1 >> Svar2;
}
void simpleCalc::display() {
    cout << "\n1 for +(Addition)\n2 for -(Subtraction)\n3 for *(Multiplication)\n4 for /(Division)\n5 for %(Modulus)\nChoose operation: ";
    int op;
    cin >> op;
    cout << "Result: ";
    switch(op) {
        case 1:
        cout << Svar1 + Svar2;
        break;

        case 2:
        cout << Svar1 - Svar2;
        break;
        
        case 3:
        cout << Svar1 * Svar2;
        break;
        
        case 4:
        if(Svar2 != 0)
        cout << Svar1 / Svar2;
        else cout << "Please enter valid input for b(denominator)." << endl;
        break;

        case 5:
        cout << (int)Svar1 % (int)Svar2;
        break;

        default:
        cout << "Please choose appropriate operation. Try Again!!" << endl;
        break;
    }
}
class scientificCalc {
    protected:
    double SCvar;
    public:
    void setDisplay();
};
int clacFactorial(int n) {
    if(n==0 || n==1) return 1;
    return n * clacFactorial(n - 1);
}
void scientificCalc::setDisplay() {
    cout << "Initializing Scientific Calculator..." << endl;
    cout << "Choose the type of operation...\n(1) for Trigonometric\n(2) for Algebraic or Logarithmic: ";
    int too;
    cin >> too;
    if(too==1) {
        cout << "Choose input type for trigo. functions\n(1) for degree\n(2) for radian.";
        int toa;
        cin >> toa;
        cout << "Choose for type of trigonometric operation.\n(1)sin(x)\t(2)cos(x)\t(3)tan(x)\n(4)sinh(x)\t(5)cosh(x)\t(6)tanx(x): ";
        int tof;
        cin >> tof;
        cout << "Enter the angle(as per your selected manner): ";
        double angle;
        cin >> angle;
        if(toa==1)
        angle *= PI / 180;
        switch (tof){
            case 1:
            cout << "Output Trigo(angle) = " << sin(angle) << endl;
            break;
            
            case 2:
            cout << "Output Trigo(angle) = " << cos(angle) << endl;
            break;
            
            case 3:
            cout << "Output Trigo(angle) = " << tan(angle) << endl;
            break;
            
            case 4:
            cout << "Output Trigo(angle) = " << sinh(angle) << endl;
            break;
            
            case 5:
            cout << "Output Trigo(angle) = " << cosh(angle) << endl;
            break;
            
            case 6:
            cout << "Output Trigo(angle) = " << tanh(angle) << endl;
            break;

            default:
            cout << "Please select an appropriate Trigonometric function. Try again!!" <<  endl;
            break; 
        }
    } else if(too == 2) {
        cout << "Choose the operation.\n(1) for 1/x(reciprocal)\t(2) for x^2(suqare)\n(3) for x!(factorial)\t(4) for log10(x)[common log]\n(5) for log(x)[natural log]\t(6) for sqrt(x)[suqare-root]: ";
        int tof;
        cin >> tof;
        cout << "Enter the value to be processed [f(x)]: x = ";
        double x;
        cin >> x;
        switch (tof){
            case 1:
            if(x!=0)
            cout << "Output [f(x)]= " << 1.0/x << endl;
            else cout << "PLease give appropriate value to calculate." <<  endl;
            break;
            
            case 2:
            cout << "Output [f(x)]= " << x * x << endl;
            break;
            
            case 3:
            cout << "Output [f(x)]= " << clacFactorial(x) << endl;
            break;
            
            case 4:
            if(x!=0)
            cout << "Output [f(x)]= " << log10(x) << endl;
            else cout << "PLease give appropriate value to calculate." <<  endl;
            break;
            
            case 5:
            if(x!=0)
            cout << "Output [f(x)]= " << log(x) << endl;
            else cout << "PLease give appropriate value to calculate." <<  endl;
            break;
            
            case 6:
            if(x>=0)
            cout << "Output [f(x)]= " << sqrt(x) << endl;
            else cout << "PLease give appropriate value to calculate." <<  endl;
            break;
            
            default:
            cout << "Please select an appropriate Trigonometric function. Try again!!" <<  endl;
            break;          
        }
    } else {
        cout << "Please slelect appropriate Math Section.... TRY AGAIN!!!" <<  endl;
    }
}
class HybridClac : public simpleCalc, public scientificCalc {

};
int main() {
    HybridClac h1;
    h1.setvalues();
    h1.display();
    h1.setDisplay();
    
    return 0;
}