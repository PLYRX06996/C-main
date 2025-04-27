#include<iostream>
using namespace std;

int main(){
    //---------Arrays in C++--------
    /*They are collection of items of same data_type
    stored in a contiguous memory location.*/
    //decleration of an Array
    int arr1[] = {1, 2, 3, 4};
    //int arr2[];//You should give the size of any array
    int n; cin>>n;
    int arr2[n];
    for (int i = 0; i < n; i++)
    {
        cin>>arr2[i];
    }
    // /*-----------Pointers and Arrays----------*/
    for (int i = 0; i < n; i++)
    {
        int* p; 
        p = &arr2[i];
        cout<<p<<" ";
    }
    //OR the pointer pointing at arr2 will have the address of the first element 
    int* q = arr2;
    for(int j=0; j<n; j++){
        //pointer arithematic
        cout<<*(q+j)<<" ";
    }
    //address(new) = address(current) + i*sizeof(data_type)
    return 0;
}