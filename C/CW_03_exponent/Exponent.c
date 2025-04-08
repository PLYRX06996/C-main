#include <stdio.h>
#include <math.h>
void main()
{
    int a, count = 0; float b;
    read: printf("Enter the value for a:");
    scanf("%d", &a);
    if(a>0){
        printf("Enter the value for b:");
        scanf("%f", &b);
        float exp = pow(a, b);
        printf("The value for a^b is:%f", exp);

        } else{ printf("Please enter a valid input.\n");
          count++;
          if(count<5){
            goto read;
        }
        }
        printf("\nEnd of computation.");
}
