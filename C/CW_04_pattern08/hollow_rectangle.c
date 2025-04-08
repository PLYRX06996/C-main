#include<stdio.h>
void main(){
    int r, c;
    printf("Enter the value for number of rows:");
    scanf("%d", &r);
    printf("Enter the value for number of columns:");
    scanf("%d", &c);
    for(int i=1;i<=r;i++){
        for(int j=1;j<=c;j++){
        if((j==1 || j==c) || (i==1 || i==r)){
            printf("*");
        } else printf(" ");
        } printf("\n");
    }

}
