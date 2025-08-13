#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

// Stack structure
struct Stack {
    int top;
    char* array[MAX_SIZE];
};

// Stack operations
struct Stack* createStack() {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->top = -1;
    return stack;
}

void push(struct Stack* stack, char* item) {
    if(stack->top >= MAX_SIZE - 1){
        return;
    }
    stack->array[++stack->top] = strdup(item);
    // Or strcpy(stack->array[stack->top], item);
}

char* pop(struct Stack* stack) {
    if (stack->top == -1) return NULL;
    return stack->array[stack->top--];
}

char* peek(struct Stack* stack) {
    if (stack->top == -1) return NULL;
    return stack->array[stack->top];
}

int isEmpty(struct Stack* stack) {
    return stack->top == -1;
}

// Helper functions
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

int precedence(char c) {
    switch (c) {
        case '^': return 3;
        case '*':
        case '/': return 2;
        case '+':
        case '-': return 1;
        default: return -1;
    }
}

char* concat(const char* str1, const char* str2, const char* op) {
    char* result = (char*)malloc(strlen(str1) + strlen(str2) + strlen(op) + 3);
    sprintf(result, "(%s%s%s)", str1, op, str2);
    return result;
}

// 1. Infix to Postfix
char* infixToPostfix(char* infix) {
    struct Stack* stack = createStack();
    char* postfix = (char*)malloc(sizeof(char) * (strlen(infix) * 2));
    int i, j = 0;
    
    for (i = 0; infix[i]; i++) {
        if (isalnum(infix[i])) {
            postfix[j++] = infix[i];
        }
        else if (infix[i] == '(') {
            char* temp = (char*)malloc(2);
            temp[0] = infix[i];
            temp[1] = '\0';
            push(stack, temp);
        }
        else if (infix[i] == ')') {
            while (!isEmpty(stack) && strcmp(peek(stack), "(") != 0) {
                char* op = pop(stack);
                postfix[j++] = op[0];
                free(op);
            }
            if (!isEmpty(stack)) {
                free(pop(stack)); // Remove '('
            }
        }
        else if (isOperator(infix[i])) {
            while (!isEmpty(stack) && strcmp(peek(stack), "(") != 0 && 
                   precedence(infix[i]) <= precedence(peek(stack)[0])) {
                char* op = pop(stack);
                postfix[j++] = op[0];
                free(op);
            }
            char* temp = (char*)malloc(2);
            temp[0] = infix[i];
            temp[1] = '\0';
            push(stack, temp);
        }
    }
    
    while (!isEmpty(stack)) {
        char* op = pop(stack);
        postfix[j++] = op[0];
        free(op);
    }
    
    postfix[j] = '\0';
    free(stack);
    return postfix;
}

// 2. Postfix to Infix
char* postfixToInfix(char* postfix) {
    struct Stack* stack = createStack();
    char temp[2] = {'\0', '\0'};
    
    for (int i = 0; postfix[i]; i++) {
        temp[0] = postfix[i];
        
        if (isalnum(postfix[i])) {
            push(stack, temp);
        }
        else if (isOperator(postfix[i])) {
            char* op2 = pop(stack);
            char* op1 = pop(stack);
            char* result = concat(op1, op2, temp);
            push(stack, result);
            free(op1);
            free(op2);
            free(result);
        }
    }
    
    char* result = strdup(pop(stack));
    free(stack);
    return result;
}

// 3. Infix to Prefix
char* infixToPrefix(char* infix) {
    // First reverse the infix expression
    int len = strlen(infix);
    char* reversed = (char*)malloc(len + 1);
    for (int i = 0; i < len; i++) {
        if (infix[i] == '(')
            reversed[len-1-i] = ')';
        else if (infix[i] == ')')
            reversed[len-1-i] = '(';
        else
            reversed[len-1-i] = infix[i];
    }
    reversed[len] = '\0';
    
    // Convert reversed infix to postfix
    char* postfix = infixToPostfix(reversed);
    free(reversed);
    
    // Reverse the postfix to get prefix
    len = strlen(postfix);
    char* prefix = (char*)malloc(len + 1);
    for (int i = 0; i < len; i++) {
        prefix[i] = postfix[len-1-i];
    }
    prefix[len] = '\0';
    free(postfix);
    
    return prefix;
}

// 4. Prefix to Infix
char* prefixToInfix(char* prefix) {
    struct Stack* stack = createStack();
    char temp[2] = {'\0', '\0'};
    
    // Read from right to left
    for (int i = strlen(prefix) - 1; i >= 0; i--) {
        temp[0] = prefix[i];
        
        if (isalnum(prefix[i])) {
            push(stack, temp);
        }
        else if (isOperator(prefix[i])) {
            char* op1 = pop(stack);
            char* op2 = pop(stack);
            char* result = concat(op1, op2, temp);
            push(stack, result);
            free(op1);
            free(op2);
            free(result);
        }
    }
    
    char* result = strdup(pop(stack));
    free(stack);
    return result;
}

// 5. Prefix to Postfix
char* prefixToPostfix(char* prefix) {
    struct Stack* stack = createStack();
    char temp[2] = {'\0', '\0'};
    
    // Read from right to left
    for (int i = strlen(prefix) - 1; i >= 0; i--) {
        temp[0] = prefix[i];
        
        if (isalnum(prefix[i])) {
            push(stack, temp);
        }
        else if (isOperator(prefix[i])) {
            char* op1 = pop(stack);
            char* op2 = pop(stack);
            int len = strlen(op1) + strlen(op2) + 2;
            char* result = (char*)malloc(len);
            sprintf(result, "%s%s%c", op1, op2, prefix[i]);
            push(stack, result);
            free(op1);
            free(op2);
            free(result);
        }
    }
    
    char* result = strdup(pop(stack));
    free(stack);
    return result;
}

// 6. Postfix to Prefix
char* postfixToPrefix(char* postfix) {
    struct Stack* stack = createStack();
    char temp[2] = {'\0', '\0'};
    
    for (int i = 0; postfix[i]; i++) {
        temp[0] = postfix[i];
        
        if (isalnum(postfix[i])) {
            push(stack, temp);
        }
        else if (isOperator(postfix[i])) {
            char* op2 = pop(stack);
            char* op1 = pop(stack);
            int len = strlen(op1) + strlen(op2) + 2;
            char* result = (char*)malloc(len);
            sprintf(result, "%c%s%s", postfix[i], op1, op2);
            push(stack, result);
            free(op1);
            free(op2);
            free(result);
        }
    }
    
    char* result = strdup(pop(stack));
    free(stack);
    return result;
}

// Example usage
int main() {
    char infix[] = "a+b*c";
    
    printf("Infix: %s\n", infix);
    
    // Test all conversions
    char* postfix = infixToPostfix(infix);
    printf("Infix to Postfix: %s\n", postfix);
    
    char* infix_from_postfix = postfixToInfix(postfix);
    printf("Postfix to Infix: %s\n", infix_from_postfix);
    
    char* prefix = infixToPrefix(infix);
    printf("Infix to Prefix: %s\n", prefix);
    
    char* infix_from_prefix = prefixToInfix(prefix);
    printf("Prefix to Infix: %s\n", infix_from_prefix);
    
    char* postfix_from_prefix = prefixToPostfix(prefix);
    printf("Prefix to Postfix: %s\n", postfix_from_prefix);
    
    char* prefix_from_postfix = postfixToPrefix(postfix);
    printf("Postfix to Prefix: %s\n", prefix_from_postfix);
    
    // Free allocated memory
    free(postfix);
    free(infix_from_postfix);
    free(prefix);
    free(infix_from_prefix);
    free(postfix_from_prefix);
    free(prefix_from_postfix);
    
    return 0;
}