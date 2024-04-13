#include <stdio.h>
#include <stdlib.h>

#include "code.h"
#include "exec.h"

void show_error(char *msg, struct InterpreterState *state) {
  struct CodeData *code_data = state->code_data;

  printf("\033[31mエラー: (%d-%d)\n", state->x, state->y);
  printf("  %s\033[39m\n", msg);

  printf("\n");
  printf("コード:\n");
  for (int i=0; i<code_data->lines; i++) {
    printf("  LINE%03d: ", i+1);
    for (int j=0; j<code_data->width_array[i]; j++) {
      if (i==state->y && j==state->x) { printf("\033[41m"); }
      printf("%c", code_data->code[i][j]);
      printf("\033[49m");
    }
    printf("\n");
  }

  free(state->code_data);
  free(state->stack->array);
  free(state->stack);
  free(state);

  exit(1);
}
