#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "error.h"

struct Stack * create_new_stack() {
  struct Stack *stack;
  stack = (struct Stack*)malloc(sizeof(*stack));
  stack->length = 0;
  stack->ALLOCATED_LENGTH = 32;
  stack->array = (int *)calloc(stack->ALLOCATED_LENGTH, sizeof(int));

  return stack;
}

void push(struct Stack *stack, int val) {
  if (stack->length >= stack->ALLOCATED_LENGTH) {
    stack->ALLOCATED_LENGTH += 32;

    int *new_arr;
    new_arr = (int *)calloc(stack->ALLOCATED_LENGTH, sizeof(int));
    for (int i=0; i<(stack->length); i++) { new_arr[i]=stack->array[i]; }
    stack->array = new_arr;
    // stack->array = (int *)realloc(stack->array, stack->ALLOCATED_LENGTH);
  }
  (stack->array)[stack->length++] = val;
}

int pop(struct Stack *stack) {
  int val;
  if (stack->length <= 0) {
    return 0;
  }

  val = (stack->array)[--stack->length];
  return val;
}

void show_stack(struct Stack *stack) {
  for (int i=0; i<(stack->length); i++) {
    printf("%d ", stack->array[i]);
  }
}
