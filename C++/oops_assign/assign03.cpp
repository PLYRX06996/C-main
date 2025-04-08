#include<iostream>
#include<conio.h>
using namespace std;

class TollBooth{
    private:
    unsigned int carsPassed, paidC, NopaidC;
    double moneyCollected;

    public:
    inline void initBooth(){
        carsPassed = 0;
        paidC = 0;
        NopaidC = 0;
        moneyCollected = 0.00;
    }

    inline void paidCars(){
        cout<<"The car passed has paid the Toll Tax."<<endl;
        paidC++;
        moneyCollected += 0.50; 
    }

    inline void nopaidCars(){
        cout<<"The car passed has not paid the Toll Tax."<<endl;
        NopaidC++;
    }

    void display(){
        carsPassed = paidC + NopaidC;
        cout<<"\nTotal cars passed through TollBooth who paid: "<<paidC<<endl;
        cout<<"Total cars passed through TollBooth who didn't paid: "<<NopaidC<<endl;
        cout<<"Total Cars passed through TollBooth: "<<carsPassed<<endl;
        cout<<"Total money collected: "<< moneyCollected<<endl;
    }
};

int main(){
    TollBooth booth;
    booth.initBooth();
    char key;

    cout<<"Press 'P' key to record the car who paid to theBooth."<<endl;
    cout<<"Press 'N' key to record the car who  didn't paid to the Booth."<<endl;
    cout<<"Press 'Esc' key to calculate total cars passed and total money collected."<<endl;
    
    while(1){
        key = _getch();

        if(key == 'P' || key == 'p'){
            booth.paidCars();
        } else if(key == 'N' || key == 'n'){
            booth.nopaidCars();
        } else if(key == 27){
            booth.display();
            break;
        } else {
            cout<<"Pressed key: ";
            char Wk = _putch(key);
            cout<<"\tPlease Enter a valid Key For TollBooth Programme."<<endl;
        }
    }
    return 0;
}