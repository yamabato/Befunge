#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"
#include "read.h"
#include "exec.h"

int main(int argc, char **argv) {
  struct CodeData *code_data;
  struct Arguments *args;

  code_data = (struct CodeData*)malloc(sizeof(*code_data));
  args = (struct Arguments*)malloc(sizeof(*args));

  parse_args(argc, argv, args);
  read_code(code_data, args);
  execute(code_data, args);

  for (int i=0; i<code_data->lines; i++) {
    free(code_data->code[i]);
  }
  free(code_data->code);
  free(code_data->width_array);
  free(code_data);

  free(args);

  return 0;
}
