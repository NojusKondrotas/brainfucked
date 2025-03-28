flags=-Wall -Wextra -O2

all: clean brainfucked

brainfucked: brainfuck-interpreter.o
	gcc $(flags) $^ -o $@

brainfuck-interpreter.o: brainfuck-interpreter.c
	gcc $(flags) -c $^

clean:
	rm -f brainfuck-interpreter.o brainfucked