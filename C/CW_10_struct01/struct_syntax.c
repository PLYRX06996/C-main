#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(){
    //Decleration
    struct Employee{
        char name[20];
        unsigned int salary;
        unsigned long int ID;
        float exp;
    };
    //Initialization
    struct Employee E1;
    printf("Enter your name:(E1)");
    gets(E1.name);
    printf("Enter your Employee ID:(E1)");
    scanf("%lu", &E1.ID);
    printf("Enter your salary:(E1)");
    scanf("%u", &E1.salary);
    printf("Enter your Industry Experience(in years):(E1)");
    scanf("%f", &E1.exp);

    //Accessing can be done as per convenient
    /*Accessing is done by (.) Dot operator*/
    return 0;
}

