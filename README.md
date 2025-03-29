# brainfucked
Brainfucked - a collection of Brainfuck interpreters.

### example programs
The infamous 'Hello World!':
```brainfuck
>++++++++[<+++++++++>-]<.>++++[<+++++++>-]<+.+++++++..+++.>>++++++[<+++++++>-]<++.------------.>++++++[<+++++++++>-]<+.<.+++.------.--------.>>>++++[<++++++++>-]<+.>>+++++[<++>-]<.
```

Truth-Machine:
```brainfuck
,>>+<<[>]>[.].
```

Fibonacci sequence (modulo-256):
```brainfuck
program has to be manually terminated
+.>+.>+<< [[->>>+>+<<<<] > [->>+>+<<<] >>.<]
```

Addition of two memory blocks:
```brainfuck
++++++>++++<[>+<-]>
```

Conditional branching, compare two inputs:
```brainfuck
output < if a is smaller than b, > if a is larger than b, and = if a is equals b
,>>,>>++++++[<++++++++++>-]>++++++[<++++++++++>-]<++>>++++++[<++++++++++>-]<+<<<<<[->>[-<]<]>>[>.[[-]>]]>>>[.>]
```