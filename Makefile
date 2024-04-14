CC = gcc
CFLAGS = -Wall
TARGET  = befunge
SRCS = befunge.c read.c exec.c stack.c error.c args.c debug.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ -lm

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)

all: clean $(OBJS) $(TARGET)

clean:
	-rm -f $(OBJS) $(TARGET) *.out
