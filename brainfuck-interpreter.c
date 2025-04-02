#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 30000

// enum op_e{
//     OP_INC_PC = '>',
//     OP_DEC_PC = '<',
//     OP_INC_VAL = '+',
//     OP_DEC_VAL = '-',
//     OP_IN = ',',
//     OP_OUT = '.',
//     OP_JMP_FWD = '[',
//     OP_JMP_BCK = ']'

// }typedef OP;

__u_short get_input(){
    char in_data[3] = {0};
    __u_short temp = 0;
    fscanf(stdin, "%3s", in_data);

    if(in_data[0] < 48 || in_data[0] > 57)
        return in_data[0];
    
    int i = 0;
    while(in_data[i] > 47 && in_data[i] < 58){
        temp *= 10;
        temp += in_data[i++] - 48;
    }

    return temp;
}

void print_memory(size_t index, size_t length, size_t memc, size_t pc, size_t loops, size_t loop_condition, __u_char *mem){
    size_t end = index + length;

    while(index <= end)
        printf("%d ", (int)mem[index++]);
    printf("| memc : %zu | value at memc : %hu | loops : %zu | loop condition : %zu | pc : %zu\n", memc, (__u_short)mem[memc], loops, loop_condition, pc);
}

int execute_bf(FILE* fptr){
    __u_char *memory = (__u_char *)calloc(MEMORY_SIZE, sizeof(__u_char));
    if(!memory){
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }
    size_t pc = 0, memc = 0;
    
    size_t loop_stack_capacity = 10, loops = 0;
    size_t *loop_counters = (size_t *)calloc(loop_stack_capacity, sizeof(size_t));
    if(!loop_counters){
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }

    char c;
    size_t invalid_loop = 0, invalid_loop_start_index = 0;

    #ifdef DEBUG
    char command;
    size_t memory_index = 0, commands_forward = 0, commands_since_start = 0;
    #endif

    while((c = fgetc(fptr)) != EOF){
        if(loops == loop_stack_capacity){
            loop_stack_capacity *= 2;
            size_t *new_data = (size_t *)realloc(loop_counters, loop_stack_capacity * sizeof(size_t));
            loop_counters = new_data;
        }

        #ifdef DEBUG
        if(commands_forward){
            printf("command: %zu\n", commands_forward);
            --commands_forward;
            goto end;
        }
        printf("\n------------------------------------------------------");
        printf("\nWaiting command: %c | pc: %zu\n", c, pc);

        command = getchar();

        if(command == 'j')
            scanf(" %zu", &commands_forward);

        while(command == 'a' || command == 'd'){
            switch(command){
                case 'a':
                    --memory_index;
                    break;
                case 'd':
                    ++memory_index;
                    break;
            }

            print_memory(memory_index, DEBUG_MEMORY_DEPTH, memc, pc, loops, invalid_loop, memory);
            scanf(" %c", &command);
        }

        while(getchar() != '\n');

        end:
        printf("\nExecuting command: %c | pc: %zu\n", c, pc);
        #endif

        if(c != ']' && c != '[' && invalid_loop){
            ++pc;
            continue;
        }

        switch (c)
        {
        case '>':
            memc = (memc + 1) % MEMORY_SIZE;
            break;
        case '<':
            memc = (memc - 1 + MEMORY_SIZE) % MEMORY_SIZE;
            break;
        case '+':
            memory[memc]++;
            break;
        case '-':
            memory[memc]--;
            break;
        case ',':   
            __u_short temp = get_input();

            while(getchar() != '\n');

            memory[memc] = temp;

            break;
        case '.':
            #ifdef DEBUG
            printf("printed value: %d\n", memory[memc]);
            #else
            putchar(memory[memc]);
            #endif

            break;
        case '[':
            loop_counters[loops++] = pc;
            if(memory[memc] == 0 && !invalid_loop){
                invalid_loop = 1;
                invalid_loop_start_index = loops;
            }
            break;
        case ']':
            if(loops == 0)
                return EXIT_FAILURE;

            if(loops == invalid_loop_start_index){
                invalid_loop = 0;
                invalid_loop_start_index = 0;
            }
            
            size_t go_back = loop_counters[--loops];

            if(memory[memc] != 0){
                pc = go_back - 1;
                fseek(fptr, go_back, SEEK_SET);
            }
            
            break;
        }

        #ifdef DEBUG
        print_memory(memory_index, DEBUG_MEMORY_DEPTH, memc, pc, loops, invalid_loop, memory);
        ++commands_since_start;
        #endif
        ++pc;
    }

    #ifdef DEBUG
    printf("\nprogram length : %zu\n", commands_since_start);
    #endif

    free(memory);
    free(loop_counters);

    return EXIT_SUCCESS;
}

int main(int argc, char **argv){
    FILE *fptr;

    if(argc < 2 || (fptr = fopen(argv[1], "r")) == NULL){
        fprintf(stderr, "No direction to a Brainfuck(.b, .bf) file: %s\n", argv[0]);
        return EXIT_FAILURE;
    }

    int status = execute_bf(fptr);

    fclose(fptr);

    return status;
}