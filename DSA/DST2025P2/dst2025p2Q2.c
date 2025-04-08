#include<stdio.h>
#include<stdbool.h>
#include<limits.h>
#include<string.h>

#define MAX 1000
#define MAX_STACKS 100
#define MAX_NAME_LENGHT 10
typedef struct Stack {
    int arr[MAX];
    int size;
    int top;
    }Stack;


void initStack(Stack *stack, int size){
        (*stack).top = -1;
        (*stack).size = size;
}

bool if_S_empty(Stack *stack){

    return (*stack).top == -1;
}

bool if_S_full(Stack *stack){

    return (*stack).top == ((*stack).size - 1);
}

int push(Stack *stack, int data){
    if(if_S_full(stack)){
        printf("stack overflow\n");
        return -1;
    }
    (*stack).top++;
    (*stack).arr[(*stack).top]= data;
    printf("%d\n", data);
    return data;
}

int pop(Stack *stack){
    if(if_S_empty(stack)){
        printf("stack underflow\n");
        return -1;
    }
    int popped = (*stack).arr[(*stack).top];
    printf("%d\n", popped);
    (*stack).top--;
    return popped;
}

int topS(Stack *stack){
        if(if_S_empty(stack)){
            printf("stack underflow\n");
            return -1;
        }
        int top = (*stack).arr[(*stack).top];
            printf("%d\n", top);                
        return top;
}

int main(){
    int tc, size, q, data, ns;
    scanf("%d", &tc);
    
    while(tc>0){
        scanf("%d %d %d", &ns, &q, &size);
        Stack stack[MAX_STACKS];
        char Name_stack[MAX_STACKS][MAX_NAME_LENGHT];
        
        for(int i=0; i<ns; i++){
            scanf("%s", Name_stack[i]);
            initStack(&stack[i], size);
        }

        while(q>0){
            char input[5], nameS[MAX_NAME_LENGHT];
            scanf("%s", input);
            if(strcmp(input, "push") == 0){
                scanf("%s %d", nameS, &data);
               for(int i=0; i<ns; i++){
                 if(strcmp(Name_stack[i], nameS)==0){
                    push(&stack[i], data);
                    break;
                 }
               }
            } else if(strcmp(input, "pop") == 0){
                scanf("%s", nameS);
               for(int i=0; i<ns; i++){
                 if(strcmp(Name_stack[i], nameS)==0){
                    pop(&stack[i]);
                    break;
                }
            }
        } else if(strcmp(input, "top") == 0){
                    scanf("%s", nameS);
               for(int i=0; i<ns; i++){
                 if(strcmp(Name_stack[i], nameS)==0){
                    topS(&stack[i]);
                    break;
                }
            }
        } 
        else printf("Please Enter a valid Stack Command");
               q--;
        }
               tc--;
    }            
}
/*for(int j=0; j<ng; j++){
            initstack(&groupSA[j], ng);
            pushForGroup(&main_stack, &groupSA[j]);
        }
        */