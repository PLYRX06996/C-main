#include<stdio.h>
void main()
{
    int n;
    printf("Enter the value for making 'Alphanumerical Triangle': ");
    scanf("%d", &n);
    for(int i=1;i<=n;i++){
        if(i%2!=0){
           int j=1;
           while(j<=i){
            printf("%d ", j);
            j++;
           } printf("\n");
        } else {
            char k =65;
            while(k<(65+i)){
                printf("%c ", k);
                k++;
            } printf("\n");
        }
    }

}
