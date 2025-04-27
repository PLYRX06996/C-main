/*----------Control Structure---------*/
/*1. Sequence structure:
Entry-->Action1->Action2->Exit*/
/*2.Selection Structure:
Entry-->Condition->((True->Action1)||(False->Action2))-->Exit*/
/*3.Loop Structure:
Entry-->[LOOP].condition((ifTrue->Action1)
->goto(LOOP.condition) (ifFalse->Action2))->Exit*/

#include<iostream>
using namespace std;
int main(){
    /*---------Selection Structure---------*/
    //1. single if() 2.if -else pair 3. if -else ladder 4. switch cases
    int age;
    cin>>age;
    if(age>0){
    if(age<18){
        cout<<"You are a minor."<<endl;
    } else if(age>=18){
        cout<<"You are an adult."<<endl;
    }
    } else {
        cout<<"Invalid Credentials!\nDon't try to make a pank!!!"<<endl;
    }

    //switch cases
    /*switch cases are only useful when
    the expression has only descrete values*/
    //So, in order to implement the above process
    int cat = 0;
    if(age>0){
        if(age<18) cat = 1;
        else cat = 2;
    }
    switch(cat){
        case 1:
        cout<<"You are a minor."<<endl;
        break;
        case 2:
        cout<<"You are an adult."<<endl;
        break;
        default:
        cout<<"Invalid Credentials!"<<endl;
        break;
    }    return 0;
}