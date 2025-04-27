#include<iostream>
using namespace std;
int add(int a,int b){
    return a+b;
}
int substr(int a,int b){
    return a-b;
}
int multipl(int a,int b){
    return a*b;
}
int div_quo(int a,int b){
    if(b==0) cout<<"'b' cannot be zero";
    return 0;
    return a/b;
}
int remain(int a,int b){
    if(b==0) cout<<"'b' cannot be zero. "<<;
    return 0;
    return a%b;
}
void operations(int a,int b){
    cout<<"The sum of a and b is:"<<add(a,b)<<endl;
    cout<<"The diff of a and b is:"<<substr(a,b)<<endl;
    cout<<"The prod of a and b is:"<<multipl(a,b)<<endl;
    cout<<"The div of a and b is:"<<div_quo(a,b)<<endl;
    cout<<"The modulo of a and b is:"<<remain(a,b)<<endl;
}
int main(){
    int a, b;
    cout<<"Enter the value for a:";
    cin>>a;
    cout<<"Enter the value for b:";
    cin>>b;
    // cout<<"The sum of a and b is:"<<a+b<<endl;
    // cout<<"The diff of a and b is:"<<a-b<<endl;
    // cout<<"The prod of a and b is:"<<a*b<<endl;
    // cout<<"The div of a and b is:"<<a/b<<endl;
    // cout<<"The modulo of a and b is:"<<a%b<<endl;
    operations(a,b);
    
    return 0;
}