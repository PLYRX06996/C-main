#include<iostream>
using namespace std;
int GCD(int* n, int* m){
    if(*n==0) return *m;
    int r = *m % *n;
    return GCD(&r, n);
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