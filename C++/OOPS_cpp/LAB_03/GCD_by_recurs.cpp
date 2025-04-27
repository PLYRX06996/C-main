#include<iostream>
using namespace std;
int GCD(int n, int m){
   if(n==0) return m;
    return GCD(n-1, m);
}
int main(){
    int n,m;
    cin>>n>>m;
    if(n<0 || m<0) {
        cout<<"Invalid Inputs";
        return 0;
    }
    // if(n<0) n*=-1;
    // if(m<0) m*=-1;
    int min, max;
    min = (n<=m) ? n : m ;
    max = (n>m) ? n : m ;
    if(min==0) {
        cout<<"The GCD of "<<n<<" and "<<m<<" is: "<<max<<endl; 
        return 0;
    }
    GCD(min, max);
    cout<<"The GCD of "<<n<<" and "<<m<<" is: "<<GCD(min, max)<<endl;
    return 0;
}