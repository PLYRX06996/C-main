#include<iostream>
using namespace std;
int GCD(int* n, int* m){
    int a = *n;
    int b = *m;
    while(b!=0){
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
int main() {   
    int n, m;
    cin>>n>>m;
    if(n<0 || m<0) {
        cout<<"Invalid Inputs";
        return 0;
    }
    
    int* min;
    int* max;
    min = (n<=m) ? &n : &m ;
    max = (n>m) ? &n : &m ;
    if(*min==0) {
        cout<<"The GCD of "<<n<<" and "<<m<<" is: "<<*max<<endl; 
        return 0;
    }
    int gcd = GCD(min,max);
    cout<<"The GCD of "<<n<<" and "<<m<<" is: "<<gcd<<endl;
    return 0;
}