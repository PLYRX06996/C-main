#include<iostream>
using namespace std;

int main(){
    /*Pointers(or pecisely pointer data_tyes) in C++*/
    //it stores the address of the other data_types
    
    int a = 3; cout<<a<<endl;
    int* b = &a; cout<<b<<endl;
    // (&)--> Address operator
    // (*)--> Dereference operator
    cout<<*b<<endl;

    //Just the basic idea of pointer to pointer OR double pointer

    //int** c = b; you cannot do this because b is a pointer to int not a pointer to pointer to int
    //Double pointer basically stores the address of a pointer
    int** c = &b;
    cout<<c<<endl<<*c<<endl<<&c<<endl;
    return 0;
}