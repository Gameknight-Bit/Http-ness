//Stack implementation :)

#include <stdlib.h>

struct stack;
typedef struct stack* stack_o;

stack_o init_stack();

size_t length(stack_o S);

void push(stack_o S, long data);

long pop(stack_o S);

long peek(stack_o S);

void free_stack(stack_o S);

