#include<stdio.h>
void main(){
    int n;
    printf("Enter the value for number of rows:");
    scanf("%d", &n);
    int m = (n+1)/2;
    if(n%2!=0){
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
                if(j==m || i==m)
                    printf("*");
                else printf(" ");
        } printf("\n");
    }
    } else printf("Enter valid Inputs.");
}
