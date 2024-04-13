#ifndef ARGS_H
#define ARGS_H

struct Arguments {
  char *file_name;

  int debug;
  int sleep;
};

void parse_args(int, char **, struct Arguments*);

#endif
