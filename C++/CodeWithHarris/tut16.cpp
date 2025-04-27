#include<iostream>
using namespace std;
void swap1(int *a, int *b){ // Formal parameter declaration
    int temp = *b;
    *b = *a;
    *a = temp;

}
void swap2(int *a, int *b){ // Formal parameter declaration
        *a = *a + *b;
        *b = *a - *b;
        *a = *a - *b;
}
void swapRefVar(int &a, int &b){ // Formal parameter declaration
    int temp = b;
    b = a;
    a = temp;
}
int& swapRetRef(int &a, int &b){ // Formal parameter declaration
    int temp = b;
    b = a;
    a = temp;
    return a;
}
int main(){
    //Call by value
    int n1, n2; // Actual parameters
    cin>>n1>>n2;
    //Call by reference using pointer variables
    cout<<"The vales of n1 and n2 before swap: "<<n1<<"\t"<<n2<<endl;
    swap1(&n1,&n2); // THis will swap a and b using pointer reference 
    cout<<"The vales of n1 and n2 after swap1: "<<n1<<"\t"<<n2<<endl;

    swap2(&n1,&n2);
    cout<<"The vales of n1 and n2 after swap2: "<<n1<<"\t"<<n2<<endl;
    // Swapping using C++ reference variables

    swapRefVar(n1, n2);
    cout<<"The vales of n1 and n2 after swap3: "<<n1<<"\t"<<n2<<endl;
    
    // Let's do something crazy return by reference
    swapRetRef(n1, n2); // swapRetRef(n1, n2) = 40; will change the value of n1 to 40 
    cout<<"The vales of n1 and n2 after swap4: "<<n1<<"\t"<<n2<<endl;
    return 0;
}