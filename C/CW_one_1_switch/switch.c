#include<stdio.h>
void main(){
    int a, b;
    char ch;
    printf("\nEnter the operator:");
    scanf("%c", &ch);
    printf("Enter the 1st operant:");
    scanf("%d", &a);
    printf("\nEnter the 2nd operant:");
    scanf("%d", &b);

    /*//Calculator using if else
    if(ch=='+') printf("%d", a+b);
    else if(ch=='-') printf("%d", a-b);
    else if(ch=='*') printf("%d", a*b);
    else if(ch=='/') printf("%d", a/b);
    else printf("Please enter valid operator.");
    */

    //Calculator using switch
    switch(ch){
    case '+':
    printf("%d", a+b);
    break;
    case '-':
    printf("\n%d", a-b);
    break;
    case '*':
    printf("\n%d", a*b);
    break;
    case '/':
    printf("\n%d", a/b);
    break;
    default:
        printf("Please enter valid operator.");
    }
}
