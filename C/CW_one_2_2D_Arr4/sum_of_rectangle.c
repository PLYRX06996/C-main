#include<stdio.h>
int main(){
    int r, c;
    printf("Enter the value for number of rows:");
    scanf("%d", &r);
    printf("Enter the value for number of columns:");
    scanf("%d", &c);
    printf("Enter the entries for 1st matrix:%dX%d", r, c);
    printf("\n");
    int arr[r][c];
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            printf("Enter a%d%d:", i+1, j+1);
            scanf("%d", &arr[i][j]);
        }
    }
    printf("\n");
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            printf("%d ", arr[i][j]);
        } printf("\n");
    }
    int r1, c1, r2, c2;
    printf("Enter the starting row index (r1): ");
    scanf("%d", &r1);
    printf("Enter the starting column index (c1): ");
    scanf("%d", &c1);
    printf("Enter the ending row index (r2): ");
    scanf("%d", &r2);
    printf("Enter the ending column index (c2): ");
    scanf("%d", &c2);

    // Validate the indices
    if (r1 < 0 || r1 >= r || c1 < 0 || c1 >= c || r2 <= r1 || r2 > r || c2 <= c1 || c2 > c) {
        printf("Invalid indices for submatrix.\n");
        return 1; // Exit with an error code
    }
    int sum=0;
    for(int i=r1;i<=r2;i++){
        for(int j=c1;j<=c2;j++){
            sum+=arr[i][j];
        }
    }
    printf("%d", sum);
    return 0;
}
