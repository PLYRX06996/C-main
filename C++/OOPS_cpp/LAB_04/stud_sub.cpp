#include<iostream>
#define MAX_SUB 3
using namespace std;

struct subject{
    int score;
};

struct student{
    subject sub[MAX_SUB];
};

int main() {
    int nstd, nsub;
    cin>>nstd>>nsub;
    student std[nstd];
    for(int i=0; i<nstd; i++){
        for(int j=0; j<nsub; j++){
            cout<<"Enter the score for "<<i+1<<"th student and for "<<j+1<<"th subject"<<endl;
                cin>>std[i].sub[j].score;
        }
    }
    
    return 0;
}