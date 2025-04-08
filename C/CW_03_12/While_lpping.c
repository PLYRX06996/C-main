#include<stdio.h>
/*void main()
{
    int i=10;
    while(i=20)
        printf("Computer buff!\n");

}*/
void main()
{
    int x=4, y=0, z;
    while(x>=0){
        --x;
        y++;
        if(x==y)
            continue;
        else
            printf("%d %d\n", x, y);
    }

}
