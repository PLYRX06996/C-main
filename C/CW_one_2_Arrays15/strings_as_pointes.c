#include<stdio.h>
void main(){
    char str[]="Memon Abdul Qaidr";
    printf("%s\n", str);
    printf("%x\n", str);
    printf("%c\n", str[0]);
    str[0]='L';
    printf("%c\n", str[0]);
    printf("%s\n", str);
    char *str1="Memon Abdul Qaidr";
    printf("%s\n", str1);
    printf("%x\n", str1);
    printf("%c\n", str1[0]);
    str1[0]='L'; //Bus Error
    printf("%c\n", str1[0]);
    printf("%s\n", str1);
    /*char *str='A';
    printf("%c\n", str);
    printf("%p", str);*/
}
