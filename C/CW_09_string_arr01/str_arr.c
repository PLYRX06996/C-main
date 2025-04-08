#include<stdio.h>
#include<string.h>
#include<math.h>
int main(){
    char str[]="College Wallah Is The Best Channel.";
    int x=sizeof((str[0]));
    printf("%d", x);//printing size of one character
    printf("\n");
    puts(str);//printing the string
    char ch='\0';
    int y=(int)ch;
    printf("%c", y);//printing null_char as character
    int* z=ch;
    printf("\n%d", z);//printing ASCII value of null_char
    int* w=&ch;
    printf("\n%d\n", w);//printing Address
    char str1[40];
    scanf("%[^\n]s", str1);
    puts(str1);

    return 0;
}
