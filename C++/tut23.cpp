/*C++ Objects Memory Allocation & using Arrays in Classes*/
#include<iostream>
using namespace std;
class Shop{
    private:
    int itemID[100];
    float itemPrice[100];
    int counter;
    public:
    void initCounter(void){ counter = 0; }
    void setPrice(void);
    void displayPrice(void);
};
void Shop :: setPrice(void){
    while(counter >= 0 && counter < 5){
        cout << "Enter your Id of item no. " << counter + 1 << endl;
        cin >> itemID[counter];
        cout << "enter the price of that item: " << endl;
        cin >> itemPrice[counter];
        counter++;
    }
}
void Shop :: displayPrice(void){
    for(int i = 0; i < counter; i++){
        cout << "The price of the item " << itemID[i] << " is " << itemPrice[i] << endl;
    }
}
int main(){
    Shop dukaan;
    dukaan.initCounter();
    dukaan.setPrice();
    dukaan.displayPrice();
    return 0;
}