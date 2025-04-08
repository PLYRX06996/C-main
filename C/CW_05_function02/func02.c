#include <stdio.h>
void india(){
    printf("You are in India.\n");
    return;
}

void england(){
    printf("You are in England.\n");
    india();
    return;
}
void australia(){
    printf("You are in Australia.\n");
    england();
    return;
}
int main(){
    australia();
    return 0;
}
