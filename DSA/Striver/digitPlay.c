#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
int lenghtOfNum(int num){
    int lenght = 0;
    while(num > 0){
        num /= 10;
        lenght++;
    }
    return lenght;
}
void reverseOfNum(int num){
    int orignum = num;
    int reversed = 0;
    // reversing the number
    while(num > 0){
        int lastdigit = num % 10;
        reversed = reversed * 10 + lastdigit;
        num /= 10;
    }
    // The TC of the above loop is O (logA(n) + 1) i.e; the number of times loop is runni'n
    // paliindrome iff origNum == revNum
    if (orignum == reversed){
        printf("%d is a palindrome number.\n", orignum);
        } else {
            printf("%d is not a palindrome and it's reverse is: %d\n", orignum, reversed);
        }
}
void ArmstrongNum(int num, int lenght){
    int orignum = num;
    int ArmSum = 0;
    while(num > 0){
        int lastdigit = num % 10;
        ArmSum = ArmSum + pow(lastdigit, lenght);
        num /= 10;
    }
    // Armstrong number is a number that is equal to the sum of cubes of its digits.
    // For example, 371 is an Armstrong number since 3*3*3 + 7*7*7 + 1*1*1 = 371
    if (ArmSum == orignum){
        printf("%d is an Armstrong number.\n", orignum);
    } else {
        printf("%d is not an Armstrong number.\n", orignum);
    }
}
void divisorOfNum(int num){
    // Innitializing all the element of an array to 'zero'
    int div[num];
    memset(div, 0, sizeof(div));
    // Checking all the divisors
    /*for (int i = 1; i <= num; i++){
        if (num % i == 0){
            div[i] = i;
        }
    }*/
   // Better implimentation
    for (int i = 1; i <= sqrt(num); i++){
        if (num % i == 0){
            int j = num / i;
            div[i] = i;
            div[j] = j;
        }
    }
    //printing all the divisors
    printf("The divisors of %d are: ", num);
    for (int i = 1; i <= num; i++){
        if (div[i] != 0){
            printf("%d ", div[i]);
        }
    }
    printf("\n");
}
void checkPrime(int num){
    int count = 0;
    for(int i = 2; i <= sqrt(num); i++){ // can also do sqrt(num)
        int j = num / i;
        if (num % i == 0 || num % j == 0) count++;
    }
    if (count == 0) printf("%d is a prime nnumber.\n", num);
    else printf("%d is not a prime number.\n", num);
}
void GCD(int n1, int n2){
    int gcd = 1;
    int min = n1 < n2 ? n1 : n2;
    for (int i = 2; i <= min; i++){
        if (n1 % i == 0 && n2 % i == 0){
            gcd = i;
        }
    }
    printf("The GCD of %d and %d is: %d\n", n1, n2, gcd);
}
// Eucledian Algorithm
// GCD(a, b) = GCD(b, a % b) or GCD(a, b) = GCD(b, a - b) where a > b 
// GCD(a, 0) = a
int GCD_Eucledian(int n1, int n2){
    if(n1 > 0 && n2 > 0) {
        return GCD_Eucledian(n2, n1 % n2);
    }
    if(n1 == 0) {
       return n2;
    }
    if(n2 == 0) {
        return n1;
    }
}
int main(){
    int n;
    // scanf("%d", &n);
    // int l = lenghtOfNum(n);
    // reverseOfNum(n);
    // ArmstrongNum(n, l);
    // // golden Point: All the divisors or factors can be found in sqrt(num)
    // // no need to go till num 
    // divisorOfNum(n);
    // checkPrime(n);
    int num1, num2;
    printf("Enter the two numbers for GCD: ");
    scanf("%d %d", &num1, &num2);
    //GCD(num1, num2);
    printf("The GCD of %d and %d is: %d", num1, num2, GCD_Eucledian(num1, num2));
    return 0;
}