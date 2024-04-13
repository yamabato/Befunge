#ifndef EXEC_H
#define EXEC_H

#include "code.h"
#include "args.h"
#include "stack.h"

enum Mode {
  EXEC,
  STRING,
  SKIP,
};

struct InterpreterState {
  int x;
  int y;
  int dx;
  int dy;

  enum Mode mode;
  struct CodeData *code_data;
  struct Stack *stack;

  int debug;
  int cx;
  int cy;
  int STACK_LINE;
  int CODE_LINE;
  int INPUT_LINE;
  int OUTPUT_LINE;
};

char get_instruction(struct InterpreterState *);
void move_pointer(struct InterpreterState *);

int proceed_next_char(struct InterpreterState *);
int exec_instruction(char, struct InterpreterState *);
int binary_oper(char, int, int, struct InterpreterState *);
void execute(struct CodeData*, struct Arguments*);

#endif
