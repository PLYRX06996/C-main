#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    typedef struct cricketer{
        char name[30];
        int age;
        int noOfmatches;
        float avg_runs;
    }cricketer;

    cricketer arr[3];
    for(int i=0;i<3;i++){
        printf("Enter the name:");
        scanf(" %[^\n]s", arr[i].name);
        printf("Enter the age:");
        scanf("%d", &arr[i].age);
        printf("Enter the No. of matches played:");
        scanf("%d", &arr[i].noOfmatches);
        printf("Enter the Average score:");
        scanf("%f", &arr[i].avg_runs);
        printf("\n");
    }

    for(int i=0;i<3;i++){
        printf("name:%s", arr[i].name);
        printf("Age:%d", arr[i].age);
        printf("No. of matches played:%d", arr[i].noOfmatches);
        printf("Average score:%f", arr[i].avg_runs);
        printf("\n");
    return 0;
}
}
