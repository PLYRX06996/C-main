#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>

#define STRLEN 100
int my_strlen(const char string[]){
    int i = 0;
    while(string[i] != '\0'){
        i++;
    }
    return(i);
}
/*Second string exists in first string.*/
int strpos(const char s1[], const char s2[]) {
    int len1 = my_strlen(s1);
    int len2 = my_strlen(s2);
    int i, j;

    for (i = 0; i <= len1 - len2; i++) {  // Iterate through s1
        for (j = 0; j < len2; j++) {
            if (s1[i + j] != s2[j])
                break;
        }
        if (j == len2)
            return(i);
    }
    return(-1);
}
/*String concatination*/
void my_strcat(char str1[], char str2[]){
    int i, j;
    for(i = 0; str1[i] != '\0'; i++);
    for(j = 0; str2[j] != '\0'; str1[i++] = str2[j++]);
    str1[i] = '\0';
}
/*extracting sub-string form str1 of j characters starting from str1[i]*/
void substr(char str1[], char str2[], int i, int j){
    int k, m;
    for(k = i, m = 0; m < j; str2[m++] = str1[k++]);
    str2[m] = '\0';
}
int main(){
    char str1[] = "Hello World!";
    char str2[] = "World!";
    printf("%d", strpos(str1, str2));
    return 0;
}