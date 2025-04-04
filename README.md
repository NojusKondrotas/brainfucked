# Brainfucked
Brainfucked - a Brainfuck interpreter with lightweight debugging capabilities. Written in C.

### DEBUG mode
Debugging can be enabled by uncommenting the DEBUG_FLAGS declaration in Makefile or by specifying DEBUG and DEBUG_MEMORY_DEPTH flags during compilation.  
Feel free to specify the desired DEBUG_MEMORY_DEPTH (the amount of memory cells to be displayed).

Navigation:
* _a_ - move memory viewport one memory cell back
* _d_ - move memory viewport one memory cell forward
* _j[number]_ - jump program execution forward by a specified number of commands

### Brainfuck programs
Here are a few Brainfuck programs, designed to demonstrate the esolang's main features.

Truth-Machine:
```brainfuck
,>>+<<[>]>[.].
```

Addition of two memory blocks:
```brainfuck
++++++>++++<[>+<-]>
```

The infamous 'Hello World!':
```brainfuck
>++++++++[<+++++++++>-]<.
>++++[<+++++++>-]<+.
+++++++..+++.
>>++++++[<+++++++>-]<++.
------------.
>++++++[<+++++++++>-]<+.
<.+++.------.--------.
>>>++++[<++++++++>-]<+.
>>+++++[<++>-]<.
```

Fibonacci sequence (modulo-256):
```brainfuck
program does not stop execution
until it's manually terminated
+.>+.>+<<[[->>>+>+<<<<]>[->>+>+<<<]>>.<]
```

Conditional branching, compare two inputs:
```brainfuck
output < if a is smaller than b,
> if a is larger than b, and = if a equals b
,>>,<++++++++++[
    >>>>++++++<++++++<++++++<<-
]
>>>+>+<+<<<<[
    ->>[-<]<
]
>>[
    >.[[-]>]
]
>>>[.>]
```

Multiplication of two memory blocks:
```brainfuck
,>,<[->[->+>+<<]>[-<+>]<<]>>>.
```