#include<stdio.h>
int main(){
    char str[]="Memon";
    char* ptr=str; //Shallow copy
    printf("%s\n", str);
    printf("%s\n", ptr);

    str[0]='L';
    printf("%s\n", str);
    printf("%s\n", ptr);

}
