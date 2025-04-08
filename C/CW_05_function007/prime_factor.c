#include<stdio.h>
#include<math.h>
int prime_fact(int n){
    int orig_num = n;
    /*Extracting all even factors in n.*/
    while(n%2==0){//do not use for()
        printf("%d ", 2);
        n/=2;
    }
    for(int i=3;i<=sqrt(n);i++){ // i<=sqrt(n) or i*i<=n; sqrt(ori_num) is wrong.
        if(n%i==0){
        printf("%d ", i);
        n/=i;
    }
}
    if(n>2) printf("%d ", n);
        return;
}
int main(){
    int n, ori_num;
    printf("Enter a positive integer:");
    scanf("%d", &n);
    ori_num=n;
    if(n>0){
    printf("The prime factors of %d are: ", ori_num);
    prime_fact(n);
    } else printf("Enter a valid number.");
    return 0;
}
