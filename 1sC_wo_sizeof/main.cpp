#include <iostream>
#include<string.h>

using namespace std;
class binarycom{
    char bin[11];
    int n;
public:

    inline void input_binary(){
    cin>>bin;
    n = strlen(bin);
    }

    inline bool validity(){
        if(n>10){
        cout << "Input exceeds 10 characters." << endl;
        return false;
    }
    else {
        for(int i=0; i<n; i++){
            if(bin[i]!='0' && bin[i]!='1'){
            cout<<"Invalid binary number."<<endl;
            return false;
        }
    }
    }   return true;
    }
        inline void complBin(){
            for(int i=0; i<n; i++){
            bin[i] = (bin[i]=='0') ? '1' : '0';
            }
    }

    inline void display(){
    cout<<"The 1's Complement of the given binary number is:"<<bin<<endl;
    }
}Binary;

int main()
{
    Binary.input_binary();
    if(Binary.validity()){
        Binary.complBin();
        Binary.display();
    }

    return 0;
}