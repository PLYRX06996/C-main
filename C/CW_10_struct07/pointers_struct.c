#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

 typedef struct pokemon{
        int HP;
        int attack;
        int speed;
        char name[20];
        char tier;
    }pokemon;

void change(pokemon* P){
    (*P).HP=70;
    (*P).attack=60;
    strcpy((*P).name, "Raichu");
    (*P).tier='S';
    (*P).speed=70;
}
int main(){
    pokemon P1;
    P1.HP=60;
    P1.attack=50;
    strcpy(P1.name, "Pikachu");
    P1.tier='A';
    P1.speed=80;

    /*pokemon* x = &P1; //Address of HP of P1
    //Changing values by Pass by Ref.
    (*x).HP=70;//() Lagana necessary hai
    (*x).speed=100;
    printf("%d", P1.HP);*/
    printf("%d\n", P1.HP);
    printf("%d\n", P1.speed);
    printf("%d\n", P1.attack);
    printf("%c\n", P1.tier);
    printf("%s\n", P1.name);

    change(&P1);

    printf("%d\n", P1.HP);
    printf("%d\n", P1.speed);
    printf("%d\n", P1.attack);
    printf("%c\n", P1.tier);
    printf("%s\n", P1.name);
    return 0;
}
