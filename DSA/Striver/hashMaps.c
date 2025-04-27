#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<limits.h>

int main() {
    int n;
    char str[20];
    scanf("%d", &n);
    for(int i = 0; i < n; i++) {
        scanf("%d", &str[i]);
    }
    int hash[256] = {0};
    for(int i = 0; i < n; i++) {
        hash[str[i]]++;
    }
    int q;
    scanf("%d", &q);
    while(q--) {
        char c;
        scanf("%c", &c);
        printf("%d\n", hash[c]);
    }

    return 0;
}