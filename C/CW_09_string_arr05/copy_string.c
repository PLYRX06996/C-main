#include<stdio.h>
#include<string.h>
#include<math.h>

int main(){
    char s1[]="Physics Wallah";
    char* s2=s1;
    //Let's Change s1
    s1[0]='M';
    printf("%s", s2);//The change Happens also in s2
    //Because address of P or s1[0] was fixed
    return 0;
}
