#include<iostream>
using namespace std;

int main(){

    float r, frac_part, n=1;
    cin>>r;
    frac_part = (r>0)? (r - (int)r) : (r - (int)r + 1);
    //cout<<frac_part;
    if(r>=0 && frac_part==0){
    for (int i = 1 ; i <=r; i++)
    { 
        for (int j = 1; j <= i ; j++)
        {
            cout<<(int)n<<" ";
            n++;
        }   
            cout<<"\n";
    }
    } else cout<<"Error!!";
    
    return 0;
  }