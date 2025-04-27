#include<iostream>
using namespace std;

// int sum(int a, int b){
//     return a+b;
// }
/*Function Prototype
type function-name (arguments)
*/
int sum(int a, int b);//-->Acceptable
// int sum1(int a, b);//-->Not Acceptable
int sum2(int, int);//-->Acceptable
void g();
void g1(void);
int main(){
    int n1, n2;
    cin>>n1>>n2;
    cout<<"The sum is: "<<sum2(n1, n2);  
    g();  
    return 0;
}

int sum2(int a, int b){
    return a+b;
}

void g(){
    cout<<"\nThis is C++ programming."<<endl;
}