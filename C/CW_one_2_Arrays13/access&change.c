#include<stdio.h>
void main(){
    char str[]="My name is Abd'Allah";
    printf("%s\n", str);
    printf("%c\n", str[0]);
    printf("%d\n", str[0]);
    //str[4]='A';
    str[4]=65;
    printf("%s\n", str);
}
