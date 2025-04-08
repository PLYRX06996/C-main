#include<iostream>
using namespace std;

int main(){
    /*---------Loop Structures----------*/
    //EVERY LOOP WILL RUN TILL THE CONDITION BECOMES FALSE
    //1.for loops 2. while loops 3.do-while loops
    int n;
    cin>>n;
    /*
    for (initialization_of_var; condition_on_var; updation_of_var)
    {
        CODE_to_be_executed
        //loop will run until the condition is true
        //when the condition is false, loop will terminate
    }
    */
    for(int i=0; i<n; i++){
        cout<<i<<endl;
    }
    /*
    initialization_of_var(); 
    while(condition_on_var){
        code_to_be_executed;
        updation_of_var;
    }*/
   int j = 0;
   while(j<n){
    cout<<j<<endl;
    j++;
   }
   /*do -while
   Basically the loop will run for atleast once,
   even if the condition is false from before starting*/
   do{
    cout<<"Hello"<<endl;
   }while(false);
    return 0;
}