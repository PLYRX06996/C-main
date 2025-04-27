#include<iostream>
#include<string.h>
using namespace std;
int main(){

    // int num, ori_num, temp, rev_num=0;
    // cin>>num;
    // ori_num=num;
    // while(num>0){
    //     temp = num%10;
    //     rev_num=rev_num*10+temp;
    //     num/=10;
    // }
    // if(rev_num==ori_num) {cout<<rev_num;}
    // else cout<<"num is not a Palindrome.";
    int n;
    cin>>n;
    char str[n], rev_str[n];
    gets(str);
    int count=0;
    for (int i = 0; i < n; i++)
    {
        rev_str[i]=(int)str[n-i];
        cout<<(int)str[n-i]<<endl;
    }
    // for (int i = 0; i < n; i++)
    // {
    //     if(rev_str[i]==str[i]) count++;
    // }
    // if(count==n) {puts(rev_str);}
    //     else cout<<"Given string is not a Palindrome.";
    
    return 0;
}