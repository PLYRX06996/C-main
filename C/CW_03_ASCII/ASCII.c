#include<stdio.h>
int main()
{
    /*Type casting*/
    int x = 65;
    while(x<=90){
        char ch = (char)x;
        printf("%c-->%d\n", x, x);
        x++;
    }
    for (int i=97; i<=122; i++){
        char ch = (char)i;
        printf("%c-->%d\n", i, i);
    }
    return 0;
}
