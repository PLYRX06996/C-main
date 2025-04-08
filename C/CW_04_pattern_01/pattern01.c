#include<stdio.h>
void main()
{
    printf("Pattern 01 stars 01 columns and rows\n");
    int r, c;
    printf("Enter the value for number of stars in rows:");
    scanf("%d", &r);
    printf("Enter the value for number of stars in columns:");
    scanf("%d", &c);
    for(int i=1;i<=r;i++){
        for(int j=1;j<=c;j++){
            printf("* ");
        }printf("\n");
    }
    printf("\nPattern:02 Number Square.\n");
    int n;
    printf("Enter the value for making 'Number Square': ");
    scanf("%d", &n);
    for(int i=1; i<=n;i++){
        for(int j=1;j<=n;j++){
            printf("%d ", j);
        } printf("\n");
    }
    printf("\nPattern:03 stars 02 increasing in row.\n");
    int p;
    printf("Enter:");
    scanf("%d", &p);
    for(int i =1;i<=p;i++){
       for(int j=1;j<=i;j++){
        printf("* ");
       } printf("\n");
    }
    printf("\nPattern:04 stars 03 decreasing in row.\n");
    int q;
    printf("Enter:");
    scanf("%d", &q);
    for(int i =q;i>=1;i--){
       for(int j=1;j<=i;j++){
        printf("* ");
       } printf("\n");
    }
    printf("\nPattern:05 numbers decreasing in row.\n");
    int s;
    printf("Enter:");
    scanf("%d", &s);
    for(int i =s;i>=1;i--){
       for(int j=1;j<=i;j++){
        printf("%d ", j);
       } printf("\n");
    }
    printf("\nPattern:06 numbers increasing in row.\n");
    int t;
    printf("Enter:");
    scanf("%d", &t);
    for(int i =1;i<=t;i++){
       for(int j=1;j<=i;j++){
        printf("%d ", j);
       } printf("\n");
    }
}
