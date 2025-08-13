#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
// Functions are set of code which performs something for you
// Functions are used to modularize youe code
// Functions are used to increase readibility
// Functions are used to use some same codes multiple times
// void --> which doesn't return anything
// Functions which returns something 
// Parameterised functions
// Non parameterised function

void inputarray(int* X[], int size){
    for(int i = 0; i < size; i++){
        scanf("%d", &X[i]);
    }
}
void printarray(int *X[], int size){
    for(int i = 0; i < size; i++){
        printf("%d ", X[i]);
    }
}
int main(){
    int n;
    printf("Enter the number of terms for Array");
    scanf("%d", &n);
    int X[n];
    inputarray(&X, n);  
    printarray(&X, n);  
    return 0;
}