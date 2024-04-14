#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"
#include "args.h"
#include "read.h"

char* read_line(FILE *fp) {
  char c;
  char *line = (char *)malloc(sizeof(char));
  int length = 1;

  while (1) {
    c = getc(fp);
    if (c == 10) { break; }
    if (c < 32 || c > 126) { break; }

    line = (char *)realloc(line, length++);
    line[length-2] = c;
  }
  line[length-1] = 0;

  return line;
}

void read_code(struct CodeData *code_data, struct Arguments *args) {
  char *line;
  char **new_code;
  int *new_width_arr;
  int lines = 0;
  int line_length = 0;
  code_data->ALLOCATED_LINES = 25;
  code_data->code = calloc(code_data->ALLOCATED_LINES, sizeof(char *));
  code_data->width_array = calloc(code_data->ALLOCATED_LINES, sizeof(int));

  FILE *fp = stdin;

  if (strcmp(args->file_name, "") != 0) {
    fp = fopen(args->file_name, "r");
  }

  for (;; lines++) {
    if (lines >= code_data->ALLOCATED_LINES) {
      code_data->ALLOCATED_LINES += 25;
      /*
      code_data->code = (char **)realloc(code_data->code, code_data->ALLOCATED_LINES);
      code_data->width_array = (int *)realloc(code_data->width_array, code_data->ALLOCATED_LINES);
      */
      new_code = (char **)calloc(code_data->ALLOCATED_LINES, sizeof(char *));
      for (int i=0; i<lines; i++) { new_code[i]=code_data->code[i]; }
      code_data->code = new_code;

      new_width_arr = (int *)calloc(code_data->ALLOCATED_LINES, sizeof(int));
      for (int i=0; i<lines; i++) { new_width_arr[i]=code_data->width_array[i]; }
      code_data->width_array = new_width_arr;
    }

    line = read_line(fp);
    line_length = strlen(line);
    if (line_length == 0) { break; }
    code_data->code[lines] = line;
    code_data->width_array[lines] = line_length;
  }
  code_data->lines = lines;
}


