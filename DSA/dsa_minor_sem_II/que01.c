#include <stdio.h>
#include <stdbool.h>
#include<string.h>
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
void push(Stack *stack, char value)
{
    if (isFull(stack))
    {
        return;
    }
    stack->top++;
    stack->arr[stack->top] = value;
}
char pop(Stack *stack)
{
    if (isEmpty(stack))
    {
        return -1;
    }
    char temp = stack->arr[stack->top];
    stack->top--;
    return temp;
}
void checkOccurances(Stack *stack)
{
    if (isEmpty(stack))
    {
        return;
    }
    int countA = 0;
    int countB = 0;
    int countC = 0;
    while (!isEmpty(stack))
    {
        char ch = pop(stack);
        switch (ch)
        {
        case -1:
            break;
        case '(':
            countA++;
            continue;
        case ')':
            countA--;
            continue;
        case '{':
            countB++;
            continue;
        case '}':
            countB--;
            continue;
        case '[':
            countC++;
            continue;
        case ']':
            countC--;
            continue;
        default:
            continue;
        }
        stack->top--;
    }
    if (countA == 0 && countB == 0 && countC == 0)
    {
        printf("The given expression is Balanced.\n");
    }
    else
    {
        printf("The given expression is Un-Balanced.\n");
    }
}
int main()
{
    char exp[MAX_SIZE];
    scanf("%s", exp);
    Stack arithm_exp;

    initStack(&arithm_exp, strlen(exp));

    for (int i = 0; i < strlen(exp); i++)
    {
        push(&arithm_exp, exp[i]);
    }

    checkOccurances(&arithm_exp);
    return 0;
}