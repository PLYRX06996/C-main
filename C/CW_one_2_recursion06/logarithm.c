#include<stdio.h>
int pow_log(int a, int b){
    if(b==0) return 1;
    int x=pow_log(a, b/2);
    if(b%2==0){ //even
        return x * x;
    } else {
    return x*x*a;
    }
}
int main(){
    int a, b;
    printf("Enter the value for a:");
    scanf("%d", &a);
    printf("Enter the value for b:");
    scanf("%d", &b);
    if(b >=0){
    int ans=pow_log(a, b);
    printf("The value of a^b=%d", ans);
    } else printf("Invalid entry.");
    return 0;
}
