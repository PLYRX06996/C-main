#include<iostream>
using namespace std;
// structures in C++ are bydefault typedefed

// Nestng of member functions in classes
class binary{
    private:
    string s;
    public:
    void read(void);
    void chk_bin(void);
    void onesC(void);
    void display(void);
};
void binary :: read(void){
    cout << "Enter a binary number:";
    cin >> s;
}
void binary :: chk_bin(void){
    for(int i = 0; i < s.length(); i++){
        if(s.at(i) != '0' && s.at(i) != '1'){
            cout << "Incorrect binary format." << endl;
            exit(0);
        }
    }
}
void binary :: onesC(void){
    chk_bin();
    for(int i = 0; i < s.length(); i++){
        if(s.at(i) == '0') s.at(i) = '1';
        if(s.at(i) == '1') s.at(i) = '0';
    }
}
void binary :: display(void){
    onesC();
    cout << "displaying the binary number: " << s << endl;
}
int main(){
    binary b;
    b.read();
    // b.chk_bin();
    // b.onesC();
    b.display();
    return 0;
}