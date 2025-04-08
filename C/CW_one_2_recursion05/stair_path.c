#include<stdio.h>
//The logic used in this problem is as that of Fibonacci Sequence
int stair(int n){
    if(n<=2) return n;
    if(n==3) return 4;
    int total_ways=stair(n-1) +stair(n-2) +stair(n-3);
    return total_ways;
    }
int main(){
    int n;
    printf("Enter n:");
    scanf("%d", &n);
    int ways=stair(n);
    printf("For stair of %d steps,number of possible ways to climb the stairs:%d", n, ways);
    return 0;
}
