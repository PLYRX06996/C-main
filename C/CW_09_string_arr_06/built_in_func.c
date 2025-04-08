#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(){
    //strlen()
    char s1[]="My Name:";
    printf("1.%d\n", strlen(s1));

    //strcpy()
    char s2[]="Abdul Qadir";
    char s3[12];
    strcpy(s3, s2);
    printf("2.%s\n", s3);

    //strcat()
    char s4[]="Abdul ";
    printf("3.%s\n", s4);
    printf("4.%p\n", s4);
    char* s5="Qadir";
    strcat(s4,s5);
    printf("5.%s\n", s4);
    printf("6.%p\n", s4);
    return 0;
}
