#include<stdio.h>
int maze(int cr, int cc, int er, int ec){
    int down_ways=0; int right_ways=0;
    if(cr==er && cc==ec) return 1;
    if(cr==er){ // Only rightway calling
        right_ways+=maze(cr, cc+1, er, ec);
    }
    if(cc==ec){
        down_ways+=maze(cr+1, cc, er, ec);
    }
    if(cr<er && cc<ec){
    down_ways+=maze(cr+1, cc, er, ec);
    right_ways+=maze(cr, cc+1, er, ec);
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
    int no_of_ways=maze(1, 1, n, m);
    printf("Number of possible ways=%d", no_of_ways);
    return 0;
}
