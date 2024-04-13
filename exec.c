#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#include "code.h"
#include "stack.h"
#include "exec.h"
#include "debug.h"
#include "error.h"

char get_instruction(struct InterpreterState *state) {
  int x = state->x;
  int y = state->y;
  char **code = state->code_data->code;
  return code[y][x];
}

void move_pointer(struct InterpreterState *state) {
  int nx = state->x;
  int ny = state->y;

  struct CodeData *code_data = state->code_data;

  int width;
  int lines;

  if (state->dx != 0) {
    nx += state->dx;
    width = code_data->width_array[state->y];
    nx = (nx + width) % width;
    state->x = nx;
  }

  if (state->dy != 0) {
    ny += state->dy;
    lines = code_data->lines;
    ny = (ny + lines) % lines;
    state->y = ny;
  }
  return;
}

int proceed_next_char(struct InterpreterState *state) {
  char instruction = get_instruction(state);

  switch (state->mode) {
    case EXEC:
      return exec_instruction(instruction, state);
    case STRING:
      if (instruction == '"') { state->mode = EXEC; }
      else { push(state->stack, (int)instruction); }
      break;
    case SKIP:
      state->mode = EXEC;
      break;
  }

  return 0;
}

int exec_instruction(char instruction, struct InterpreterState *state) {
  int val;
  int val_a;
  int val_b;

  int digits;

  int int_input;
  char char_input;

  int width;

  switch (instruction) {
    case '<':
      state->dx = -1;
      state->dy = 0;
      break;
    case '>':
      state->dx = 1;
      state->dy = 0;
      break;
    case '^':
      state->dx = 0;
      state->dy = -1;
      break;
    case 'v':
      state->dx = 0;
      state->dy = 1;
      break;
    case '_':
      val = pop(state->stack);
      state->dy = 0;
      if (val == 0) { state->dx = 1; }
      else { state->dx = -1; }
      break;
    case '|':
      val = pop(state->stack);
      state->dx = 0;
      if (val == 0) { state->dy = 1; }
      else { state->dy = -1; }
      break;
    case '?':
      srand((unsigned int)time(NULL));
      switch (rand()%4) {
        case 0:
          state->dx = 1;
          state->dy = 0;
          break;
        case 1:
          state->dx = -1;
          state->dy = 0;
          break;
        case 2:
          state->dx = 0;
          state->dy = 1;
          break;
        case 3:
          state->dx = 0;
          state->dy = -1;
          break;
      }
    case ' ':
      break;
    case '#':
      state->mode = SKIP;
      break;
    case '@':
      return 1;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      push(state->stack, (int)instruction-48);
      break;
    case '"':
      state->mode = STRING;
      break;

    case '&':
      if (state->debug) {
        printf("\033[%d;%dH", 5+state->code_data->lines, 8);
      }
      scanf("%d", &int_input);
      if (state->debug) {
        printf("\033[%d;%dH", state->INPUT_LINE, 8);
        printf("\033[0K");
      }
      push(state->stack, int_input);
      break;
    case '~':
      if (state->debug) {
        printf("\033[%d;%dH", state->INPUT_LINE, 8);
      }
      char_input = getc(stdin);
      if (state->debug) {
        printf("\033[%d;%dH", 5+state->code_data->lines, 8);
        printf("\033[0K");
      }
      push(state->stack, (int)char_input);
      break;
    case '.':
      val = pop(state->stack);

      if (val > 0) {
        digits = (int)(log10((double)val)+1);
      } else if (val == 0) {
        digits = 1;
      } else {
        digits = (int)(log10(-(double)val)+2);
      }
      state->cx += digits+1;

      printf("%d ", val);
      break;
    case ',':
      val = pop(state->stack);
      printf("%c", val);
      if (val == 10) { state->cx=0; state->cy++; }
      else { state->cx++; }
      break;

    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '`':
      val_a = pop(state->stack);
      val_b = pop(state->stack);

      push(state->stack, binary_oper(instruction, val_a, val_b, state));
      break;

    case '!':
      val = pop(state->stack);
      if (val == 0) { push(state->stack, 1); }
      else { push(state->stack, 0); }
      break;

    case ':':
      val = pop(state->stack);
      push(state->stack, val);
      push(state->stack, val);
      break;
    case '\\':
      val_a = pop(state->stack);
      val_b = pop(state->stack);
      push(state->stack, val_a);
      push(state->stack, val_b);
      break;
    case '$':
      pop(state->stack);
      break;

    case 'g':
      val_a = pop(state->stack);
      val_b = pop(state->stack);

      if (val_a < 0 || state->code_data->lines <= val_a) {
        show_error("g: 範囲外の座標(Y)", state);
      }
      width = state->code_data->width_array[val_a];
      if (val_b < 0 || width <= val_b) {
        show_error("g: 範囲外の座標(X)", state);
      }

      push(state->stack, state->code_data->code[val_a][val_b]);
      break;

    case 'p':
      val_a = pop(state->stack);
      val_b = pop(state->stack);
      val = pop(state->stack);

      if (val_a < 0 || state->code_data->lines <= val_a) {
        show_error("p: 範囲外の座標(Y)", state);
      }
      width = state->code_data->width_array[val_a];
      if (val_b < 0 || width <= val_b) {
        show_error("p: 範囲外の座標(X)", state);
      }

      state->code_data->code[val_a%state->code_data->lines][val_b] = (char)val;
      break;
  }

  return 0;
}

int binary_oper(char instruction, int val_a, int val_b, struct InterpreterState *state) {
  switch (instruction) {
    case '+':
      return val_a + val_b;
    case '-':
      return val_b - val_a;
    case '*':
      return val_a * val_b;
    case '/':
      if (val_a == 0) { show_error("/: ゼロ除算", state); }
      return (int)(val_b / val_a);
    case '%':
      if (val_a == 0) { show_error("%: ゼロ除算", state); }
      return (int)(val_b % val_a);
    case '`':
      if (val_b > val_a) { return 1; }
      return 0;
  }

  return 0;
}

void execute(struct CodeData *code_data, struct Arguments *args) {
  struct InterpreterState *state;
  int exit_code = 0;

  state = (struct InterpreterState*)malloc(sizeof(*state));
  state->x = 0;
  state->y = 0;
  state->dx = 1;
  state->dy = 0;

  state->mode = EXEC;
  state->code_data = code_data;
  state->stack = create_new_stack();

  state->debug = args->debug;
  state->cx = 0;
  state->cy = 0;
  state->STACK_LINE = 1;
  state->CODE_LINE = 3;
  state->INPUT_LINE = state->CODE_LINE + 2 + state->code_data->lines;
  state->OUTPUT_LINE = state->INPUT_LINE + 2;

  if (state->debug) {
    printf("\033[2J");
    printf("\033[%d;1HSTACK:", state->STACK_LINE);
    printf("\033[%d;1HCODE:", state->CODE_LINE);
    printf("\033[%d;1HINPUT:", state->INPUT_LINE);
    printf("\033[%d;1HOUTPUT:", state->OUTPUT_LINE);
  }
  while (1) {
    if (state->debug) {
      debug_show_stack(state);
      debug_show_code(state);

      printf("\033[%d;%dH", state->cy+state->OUTPUT_LINE+1, state->cx+1);
      printf("\033[34m");
    }

    exit_code = proceed_next_char(state);
    printf("\033[39m");

    if (exit_code != 0) {
      break;
    }

    move_pointer(state);

    usleep(args->sleep * 1000);
  }

  if (state->debug) { printf("\n"); }

  free(state->stack->array);
  free(state->stack);
  free(state);
}
