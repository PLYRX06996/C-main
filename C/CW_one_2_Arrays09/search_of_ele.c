#include<stdio.h>
#include<stdbool.h>
int main(){
    int n;
    printf("Enter the value for number of elements:");
    scanf("%d", &n);
    int arr[n];
    n=sizeof(arr)/4;

    for(int i=0;i<n;i++){
        arr[i]=i+1;
    }
    int x;
    bool flag=false;
    printf("Enter the value for x:");
    scanf("%d", &x);
    for(int j=0;j<n;j++){
        if(arr[j]==x){
        flag=true;
        break;
        }
    }
    if(flag==false) printf("Does not Exist!");
        else printf("Exists!");
}
