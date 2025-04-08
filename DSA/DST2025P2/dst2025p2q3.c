#include<stdio.h>
#include<stdbool.h>
#include<limits.h>
#include<string.h>
#include<math.h>

#define MAX_NStud 1000
#define MAX_NG 100
#define MAX_SG 100
#define MAX_SRoll 11

typedef struct{
    float cg;
    char roll[MAX_SRoll];
}student;
typedef struct{
    student S[MAX_NStud];
    int size;
    int top;
}Stack;

void initstack(Stack *stack, int n){
    stack->top = -1;
    stack->size = n;
}

bool if_S_empty(Stack *stack){

    return stack->top == -1;
}

bool if_S_full(Stack *stack){

    return stack->top == (stack->size - 1);
}

int push(Stack *stack, student *s){
    if(if_S_full(stack)){
        return -1;
    }
    stack->top++;
    // strcpy(stack->S[stack->top].roll, s->roll);
    // stack->S[stack->top].cg = s->cg;  
    return 0;
}

int pop(Stack *stack, student *popped){
    if(if_S_empty(stack)){
        return -1;
    }
        *popped = stack->S[stack->top];
        stack->top--;
        return 0;
}

student topS(Stack *stack) {
    student temp;
    temp.cg = -1;
    strcpy(temp.roll, "");
    return if_S_empty(stack) ? temp : stack->S[stack->top];
}

int GroupFind(Stack *group, student *check, int ng){
    int i=-1;
    float D=-1;
    for(int j=0; j<ng; j++){
        if(if_S_full(&group[j])) continue;

        student topStudent = topS(&group[j]);
        float d = fabsf(check->cg - topStudent.cg);
        if(d>D || D==-1){
            D = d;
            i = j;
        } 
    }
    return i;
}

int main()
{
    int tc;
    scanf("%d",&tc);
    while(tc-- ){
        int nstud, ng, ms;
        scanf("%d %d %d",&nstud,&ng,&ms);
        Stack main_stack;
        // student S[nstud];
        Stack groupSA[MAX_NG];
        
        // initstack(&main_stack, nstud);
        initstack(&main_stack, nstud);
        
        for(int i=0; i<nstud; i++){
            student temp;
            temp.cg = -1;
            strcpy(temp.roll, "");
            scanf("%s", temp.roll);
            scanf("%f", &temp.cg);
            //push(&main_stack,S);
            push(&main_stack, &temp);
        }

        for(int j=ng-1; j>=0; j--){
            initstack(&groupSA[j], ms);
            student popped;
            if(pop(&main_stack, &popped)==0) push(&groupSA[j], &popped);
    }

        while(!if_S_empty(&main_stack)){
            student top;
            if(pop(&main_stack, &top)==0){
            int group = GroupFind(groupSA, &top, ng);
            if(group != -1){
                push(&groupSA[group], &top);
            }
        }
        }

        for(int l=ng-1; l>=0; l--){
            student s;
            while(!if_S_empty(&groupSA[l])){
                if(pop(&groupSA[l], &s)==0) printf("%s ", s.roll);
            }
            printf("\n");
        }
}
        return 0;
}