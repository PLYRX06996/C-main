#include<stdio.h>
#include<string.h>
#include<math.h>

void main(){
    char str[] = "Gulam Mohiyuddin.";
    char* ptr = str;
    printf("1.%s\n", ptr);
    printf("2.%p\n", ptr);
    printf("3.%p\n", str[0]);
    ptr="Abdul Qadir";
    printf("4.%s\n", str);

    int i= 0;
    /*while(str[i]!='\0'){
        printf("%c", str[i]);
        i++;
    }*/
    str[0]='J';
    printf("5.");
    while(*ptr!='\0'){
        printf("%c", *ptr);
        ptr++;
        i++;
    }

}
