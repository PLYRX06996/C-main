#include<stdio.h>
void change(int x[]){
    int m=sizeof(x)/4;
    printf("%d\n", m);
    x[2]=90;
}
int main(){
    int arr[] = {5,2,9,3,4};
    int n=sizeof(arr)/4;
    printf("%d\n", n);
    printf("%d\n", arr[2]);
    change(arr);
    printf("%d", arr[2]);
    return 0;
}
/*void change(int x){
    x=4;
}
void main(){
    int x= 6;
    printf("%d\n", x);
    change(x);
    printf("%d", x);
}*/
