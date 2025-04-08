#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

void pattern01(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("* ");
        }
        printf("\n");
    }
    printf("\n");
}
void pattern02(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            printf("* ");
        }
        printf("\n");
    }
    printf("\n");
}
void pattern03(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            printf("%d ", j + 1);
        }
        printf("\n");
    }
    printf("\n");
}
void pattern04(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            printf("%d ", i + 1);
        }
        printf("\n");
    }
    printf("\n");
}
void pattern05(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n - i - 1; j++) {
            printf("* ");
        }
        printf("\n");
    }
    printf("\n");
}
void pattern06(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n - i - 1; j++) {
            printf("%d ", j + 1);
        }
        printf("\n");
    }
    printf("\n");
}
/*   1 2 3 4 5 6 7 8 9 
   1         * 
   2       * * * 
   3     * * * * * 
   4   * * * * * * * 
   5 * * * * * * * * *  
    n*/
void pattern07(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n + i; j++) {
            if (j >= n - i) {
                printf("*");
        } else printf(" ");
    }
    printf("\n");
    }
    printf("\n");
}
void pattern08(int n) {
    for (int i = 0; i < n; i++) {
        for (int j =0; j <= 2*n - i - 1; j++) {
            if (j > i) {
                printf("*");
        } else printf(" ");
    }
    printf("\n");
    }
    printf("\n");
}
void pattern09(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n + i; j++) {
            if (j >= n - i) {
                printf("*");
        } else printf(" ");
    }
    printf("\n");
    }
    for (int i = 1; i < n; i++) {
        for (int j =0; j <= 2*n - i - 1; j++) {
            if (j > i) {
                printf("*");
        } else printf(" ");
    }
    printf("\n");
    }
}
void pattern10(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            printf("* ");
        }
        printf("\n");
    }
    for (int i = 1; i < n; i++) {
        for (int j = 0; j <= n - i - 1; j++) {
            printf("* ");
        }
        printf("\n");
    }
    printf("\n");
}
int main(){
    int n;
    scanf("%d", &n);
    pattern01(n);
    pattern02(n);
    pattern03(n);
    pattern04(n);
    pattern05(n);
    pattern06(n);
    pattern07(n);
    pattern08(n);
    pattern09(n);
    pattern10(n);
    return 0;
}
