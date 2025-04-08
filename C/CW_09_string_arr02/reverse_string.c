#include<stdio.h>
#include<string.h>
#include<math.h>

int main(){
    char str[20];
    puts("Enter the string:");
    gets(str);//scanf("%[^\n]s", str);
    int i=0;
    int size=0;
    while(str[i]!='\0'){
        size++;
        i++;
    }
    for(int j=0, k=size-1;j<=k;j++,k--){
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
    puts("The reversed string is:");
    puts(str);
    return 0;
}
