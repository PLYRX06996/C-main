#include<stdio.h>
void main()
{
    int num, reversed_num = 0;
    printf("Enter a number:");
    scanf("%d", &num);
    int original_num = num;
    while(num!=0){
        int last_digit = num%10;
        reversed_num = reversed_num*10 + last_digit;
        num = num/10; //or num/ =10;
    }
    if (original_num !=reversed_num)
        printf("The given number is not a Palindrome and hence reversed number is:%d\n", reversed_num);
    else  printf("The given number is a Palindrome.");

}
