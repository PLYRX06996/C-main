#include <stdio.h>

int main() {
    int a = 14;           // Step 1: Declare an integer variable 'a' and initialize it to 25.
    int *x = &a;         // Step 2: Declare a pointer 'x' and assign it the address of 'a'.
    int **y = &x;        // Step 3: Declare a pointer to a pointer 'y' and assign it the address of 'x'.

    printf("%d\n", a);   // Step 4: Print the value of 'a'.
    printf("%p\n", x);   // Step 5: Print the address stored in 'x' (address of 'a').
    printf("%p\n", *x);  // Step 6: Print the value stored at the address pointed to by 'x' in hexadecimals (which is the address of 'a').
    printf("%d\n", *x);  // Step 7: Print the value pointed to by 'x' (which is the value of 'a').
    printf("%p\n", y);   // Step 8: Print the address stored in 'y' (address of 'x').
    printf("%p\n", *y);  // Step 9: Print the value stored at the address pointed to by 'y' (which is the address of 'a').
    printf("%p\n", **y); // Step 10: Print the value stored at the address pointed to by 'y' in hexadecimals (which is the address of 'a').
    printf("%d", **y);   // Step 11: Print the value pointed to by the pointer pointed to by 'y' (which is the value of 'a').

    return 0;           // Step 12: Return 0, indicating successful completion of the program.
}
