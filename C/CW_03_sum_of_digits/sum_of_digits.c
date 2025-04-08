#include<stdio.h>
void main()
{
    int n;
    printf("Enter an integer:");
    scanf("%d", &n);
    int sum = 0, Evensum = 0, count = 0, Oddsum=0;
    while(n!=0){
        int last_digit = n%10;
        n = n/10;
        count++;
        sum = sum + last_digit;
        if(count%2==0){
            Evensum = Evensum + last_digit;
        }
        if(count%2!=0){
            Oddsum = Oddsum + last_digit;
        }
    }

    printf("The sum of even place digits of a given number is:%d\n", Evensum);
    printf("The sum of odd place digits of a given number is:%d\n", Oddsum);
    printf("The sum of all digits of a given number is:%d\n", sum);
}
