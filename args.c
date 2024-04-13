#include <stdlib.h>
#include <string.h>

#include "args.h"

void parse_args(int argc, char **argv, struct Arguments *args) {
  args->file_name = "";

  for (int i=1; i<argc; i++) {
    if (strcmp(argv[i], "-d") == 0) {
      args->debug = 1;
    } else if(strcmp(argv[i], "-s") == 0) {
      args->sleep = atoi(argv[++i]);
    } else {
      args->file_name = argv[i];
    }
  }
}
