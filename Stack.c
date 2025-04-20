#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>

struct stack {
    struct stack *next;
    long data;
};

typedef struct stack *stack_o;

stack_o init_stack(){
    stack_o S = malloc(sizeof(struct stack));
    if (S == NULL) {
        printf("Error: Malloc failure.");
        exit(1);
    }
    S->next = NULL;
    S->data = -1;
    return S;
};

void push(stack_o S, long data) {
    stack_o S2 = init_stack();
    S2->next = S->next;
    S2->data = S->data;
    S->next = S2;
    S->data = data;
}

long pop(stack_o S){
    if (S == NULL | S->next == NULL) {
        printf("Error: Stack is empty");
        exit(1);
    }
    long payload = S->data;
    S->data = S->next->data;
    stack_o hold = S->next;
    S->next = S->next->next;
    return payload;
}

long peek(stack_o S){
    if (S == NULL || S->next == NULL) {
        printf("Error: Stack is empty");
        exit(1);
    }
    return S->data;
}

void free_stack(stack_o S){
    stack_o ptr = S;
    while (ptr != NULL) {
        stack_o tmp = ptr->next;
        free(ptr);
        ptr = tmp;
    } 
}