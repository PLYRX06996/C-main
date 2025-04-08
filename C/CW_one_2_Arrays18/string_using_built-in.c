#include<stdio.h>
#include<string.h>
int main(){
    char str[]="Memon Abdul";
    char* ptr;
    printf("%s\n", ptr);
    strcpy(ptr, str); //Deep copy
    printf("%s\n", str);
    printf("%s\n", ptr);

    str[0]='L';
    printf("%s\n", str);
    printf("%s\n", ptr);
}
