CC=gcc
DEBUG_FLAGS=-D DEBUG -D DEBUG_MEMORY_DEPTH=10
CFLAGS=-Wall -Wextra -O2 $(DEBUG_FLAGS)

all: clean brainfucked

brainfucked: brainfuck-interpreter.o
	$(CC) $(CFLAGS) $^ -o $@

brainfuck-interpreter.o: brainfuck-interpreter.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o brainfucked