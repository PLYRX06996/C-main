#include<stdio.h>
int maze(int n, int m){
    int down_ways=0; int right_ways=0;
    if(n==1 && m==1) return 1;
    if(n==1){ // Only rightway calling
        right_ways+=maze(n, m-1);
    }
    if(m==1){
        down_ways+=maze(n-1, m);
    }
    if(n>1 && m>1){
    down_ways+=maze(n, m-1);
    right_ways+=maze(n-1, m);
    }
    int total_ways = right_ways + down_ways;
    return total_ways;
}
int main(){
    int n, m;
    printf("Enter the value for number of rows:");
    scanf("%d", &n);
    printf("Enter the value for number of columns:");
    scanf("%d", &m);
    int no_of_ways=maze(n, m);
    printf("Number of possible ways=%d", no_of_ways);
    return 0;
}
