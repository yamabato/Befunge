#include <stdio.h>

#include "debug.h"
#include "stack.h"

void debug_show_stack(struct InterpreterState *state) {
  printf("\033[%d;8H\033[0K", state->STACK_LINE);
  printf("\033[32m");
  show_stack(state->stack);
  printf("\033[39m");
}

void debug_show_code(struct InterpreterState *state) {
  char c;

  printf("\033[%d;1H\033[2K", state->CODE_LINE+1);
  for (int i=0; i<state->code_data->lines; i++) {
    for (int j=0; j<state->code_data->width_array[i]; j++) {
      if (i==state->y && j==state->x) { printf("\033[47m"); }

      c = state->code_data->code[i][j];
      if (c<32 || c>126) { printf("\033[33m?\033[39m"); }
      else { printf("\033[36m%c\033[39m", c); }

      if (i==state->y && j==state->x) { printf("\033[49m"); }
    }
    printf("\n");
  }
}
