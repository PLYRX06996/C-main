#include<stdio.h>
#include<string.h>
#include<math.h>

int main(){
    //Exmple 01
    char* ptr ="College Wallah";
    printf("%c\n", *ptr);
    printf("String 1:%s\n", ptr);
    printf("Address 1:%p\n", ptr);
    ptr="JEE Wallah";
    printf("String 2:%s\n", ptr);
    printf("Address 2:%p\n", ptr);

    //Example 02
    char str[]="Physics Wallah";
    str[0]='M';
    printf("%s", str);
    return 0;
}
