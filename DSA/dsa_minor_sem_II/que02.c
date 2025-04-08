#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_SIZE 100
typedef struct Stack
{
    int top;
    int size;
    char arr[MAX_SIZE];
} Stack;

void initStack(Stack *stack, int cap)
{
    stack->top = -1;
    stack->size = cap;
}
bool isEmpty(Stack *stack)
{
    return stack->top == -1;
}
bool isFull(Stack *stack)
{
    return stack->top == stack->size - 1;
}
void pushS(Stack *stack, char ch)
{
    if (isFull(stack))
        return;
    stack->arr[++stack->top] = ch;
}
char topS(Stack *stack)
{
    if (isEmpty(stack))
    {
        return -1;
    }
    return stack->arr[stack->top];
}
char popS(Stack *stack)
{
    if (isEmpty(stack))
    {
        return 0;
    }
    return stack->arr[stack->top--];
}

void revNprintStr(Stack *stack)
{
    while (!isEmpty(stack))
    {
        char temp = popS(stack);
        if (isalpha(temp))
        {
            printf("%c", temp);
        }
    }
}

int main()
{
    char str[MAX_SIZE];
    scanf("%s", str);
    Stack string;
    initStack(&string, strlen(str));
    for (int i = 0; i < strlen(str); i++)
    {
        pushS(&string, str[i]);
    }
    revNprintStr(&string);
    return 0;
}