#ifndef STACK_H
#define STACK_H

struct Stack {
  int ALLOCATED_LENGTH;
  int length;
  int *array;
};

struct Stack * create_new_stack();
void push(struct Stack *, int);
int pop(struct Stack *);
void show_stack(struct Stack *);

#endif
