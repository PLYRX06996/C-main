#include<stdio.h>
void main()
{
    /*int n;
    printf("Enter a number : ");
    scanf("%d", &n);
    int a = 0;
    for(int i=2;i<n;i++){
        if(n%i==0){
            a = 1;
            break;
        }
        } if(a==0) {
            printf("The given number is prime.\n");
        } else {
            printf("The given number is composite.\n");
        }*/
    int x = 4, y, z;
    z = x--;
    y = --x;
    printf("%d\n%d\n%d\n", x, y, z);
}
