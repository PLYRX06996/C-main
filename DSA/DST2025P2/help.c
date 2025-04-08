#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <math.h>

#define MAX_NStud 1000
#define MAX_NG 100
#define MAX_SG 100
#define MAX_SRoll 11

typedef struct {
    int top;
    int size;
    float cg[MAX_NStud];
    char roll[MAX_NStud][MAX_SRoll];
} Stack;

void initializeStack(Stack *stack, int n) {
    stack->top = -1;
    stack->size = n;
}

bool ifStackEmpty(Stack *stack) {
    return stack->top == -1;
}

bool ifStackFull(Stack *stack) {
    return stack->top == stack->size - 1;
}

void push(Stack *stack, float cg, char roll[MAX_SRoll]) {
    if (ifStackFull(stack)) {
        return;
    }
    stack->top++;
    strcpy(stack->roll[stack->top], roll);
    stack->cg[stack->top] = cg;
}

void pop(Stack *stack) {
    if (ifStackEmpty(stack)) {
        return;
    }
    stack->top--;
}

int findBestGroup(Stack groups[], int ng, float currentCG) {
    int bestGroup = -1;
    float maxDiff = -1;

    for (int i = 0; i < ng; i++) {
        if (!ifStackFull(&groups[i])) {
            float topCG = groups[i].cg[groups[i].top];
            float diff = fabs(topCG - currentCG);
            if (diff > maxDiff) {
                maxDiff = diff;
                bestGroup = i;
            }
        }
    }
    return bestGroup;
}

void printGroups(Stack groups[], int ng) {
    for (int i = 0; i < ng; i++) {
        while (!ifStackEmpty(&groups[i])) {
            printf("%s ", groups[i].roll[groups[i].top]);
            pop(&groups[i]);
        }
        printf("\n");
    }
}

int main() {
    int tc;
    scanf("%d", &tc);
    while (tc--) {
        int nstud, ng, mspg;
        scanf("%d %d %d", &nstud, &ng, &mspg);
        
        Stack students;
        Stack groups[MAX_NG];

        initializeStack(&students, nstud);

        for (int i = 0; i < nstud; i++) {
            float tempCG;
            char tempROLL[MAX_SRoll];
            scanf("%s %f", tempROLL, &tempCG);
            push(&students, tempCG, tempROLL);
        }

        for (int i = 0; i < ng; i++) {
            initializeStack(&groups[i], mspg);
            if (!ifStackEmpty(&students)) {
                float cg = students.cg[students.top];
                char roll[MAX_SRoll];
                strcpy(roll, students.roll[students.top]);
                pop(&students);
                push(&groups[i], cg, roll);
            }
        }

        while (!ifStackEmpty(&students)) {
            float currentCG = students.cg[students.top];
            char currentRoll[MAX_SRoll];
            strcpy(currentRoll, students.roll[students.top]);
            pop(&students);

            int bestGroup = findBestGroup(groups, ng, currentCG);
            if (bestGroup != -1) {
                push(&groups[bestGroup], currentCG, currentRoll);
            }
        }

        printGroups(groups, ng);
    }
    return 0;
}