#include<stdio.h>
#include<stdbool.h>
#include<limits.h>
#define MAX 1000

typedef struct Stack {
    int arr[MAX];
    int size;
    int top;
    }stack;

void initStack(struct Stack *stack, int size){
        stack->top = -1;
        stack->size = size;
}

bool if_S_empty(struct Stack *stack){

    return stack->top == -1;
}

bool if_S_full(struct Stack *stack){

    return stack->top == (stack->size - 1);
}

int push(struct Stack *stack, int data){
    if(if_S_full(stack)){
        printf("stack overflow\n");
        return 0;
    }
    stack->top++;
    int pushed = stack->arr[stack->top]= data;
    printf("%d\n", pushed);
    return pushed;
}

int pop(struct Stack *stack){
    if(if_S_empty(stack)){
        printf("stack underflow\n");
        return 0;
    }
    int popped = stack->arr[stack->top];
    printf("%d\n", popped);
    stack->top--;
    return popped;
}

int topS(struct Stack *stack){
        if(if_S_empty(stack)){
            printf("stack underflow\n");
            return -1;
        }
            printf("%d\n", stack->arr[stack->top]);                
        return stack->arr[stack->top];
}

int main(){
    int tc, size, q, data;
    scanf("%d", &tc);
    char input[10];
    struct Stack stack;

    while(tc>0){
        scanf("%d %d", &q, &size);
        initStack(&stack, size);
        while(q>0){
            scanf("%s", input);
            if(input[0]=='p' && input[1]=='o'){
                    pop(&stack);
            }
            else if(input[0]=='p' && input[1]=='u'){
                scanf("%d", &data);
                push(&stack, data);
        }
        else if(input[0]=='t'){
                topS(&stack);
        }
        else printf("Please Enter a valid Stack Command");
            q--;
    }
            tc--;   
    }
    return 0;
}