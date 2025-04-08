#include<stdio.h>
int main()
{
   int x;
   printf("Enter the marks:");
   scanf("%d", &x);
   if (x>=0 && x<=100){
    if (x>=91){
        printf("Excellent");
    } else if (x>=81) {
           printf("Very good");
        } else if (x>=71){
            printf("Good");
            } else if (x>=61){
            printf("can do better");
            } else if (x>=51){
            printf("Average");
            } else if (x>=41) {
            printf("Below average");
            } else {
            printf("Fail");
           } } else {
   printf("Invalid Entry");
   }


    return 0;
}

