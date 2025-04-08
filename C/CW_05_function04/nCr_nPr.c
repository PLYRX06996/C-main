#include<stdio.h>
int nfact(int n){
    int nfact=1;
    for(int i=n; i>=1;i--){
        nfact=nfact*i;
    }
    return nfact;
}

int rfact(int r){
    int rfact=1;
    for(int i=r; i>=1;i--){
        rfact=rfact*i;
    }
    return rfact;
}

int nrfact(int n, int r){
    int nrfact=1;
    for(int i=n-r; i>=1;i--){
        nrfact=nrfact*i;
    }
    return nrfact;
}
int main(){
    int n, r;
    printf("Enter the value for n and r: ");
    scanf("%d %d", &n, &r);
    if(n>=r){
    int nCr = nfact(n)/(rfact(r)*nrfact(n, r));
    printf("The value of nCr is: %d\n", nCr);
    int nPr = nfact(n)/nrfact(n, r);
    printf("The value of nPr is: %d", nPr);
    int m;
    printf("\nEnter the value for number of rows in Pascal Triangle: ");
    scanf("%d", &m);
    int nsp=m-1;
    for(int i=0;i<=m-1;i++){
            for(int k=nsp;k>=0;k--){
                printf(" ");
            } nsp--; int first=1;
        for(int j=0;j<=i;j++){
                printf("%d ", first);
                first = first*(i-j)/(j+1);
        }
        printf("\n");
    }
    } else printf("Invalid Inputs.");
    return 0;
}

