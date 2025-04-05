#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 30000

enum e_op{
    OP_INC_PC = '>',
    OP_DEC_PC = '<',
    OP_INC_VAL = '+',
    OP_DEC_VAL = '-',
    OP_IN = ',',
    OP_OUT = '.',
    OP_JMP_FWD = '[',
    OP_JMP_BCK = ']'

};
typedef enum e_op OP;

__u_short get_input(){
    char in_data[3] = {0};
    __u_short num_val = 0;
    fscanf(stdin, "%3s", in_data);

    if(in_data[0] < 48 || in_data[0] > 57)
        return in_data[0];
    
    int i = 0;
    while(in_data[i] > 47 && in_data[i] < 58){
        num_val *= 10;
        num_val += in_data[i++] - 48;
    }

    return num_val;
}

void print_memory(size_t index, size_t length, size_t memc, size_t pc, size_t loops, size_t loop_condition, __u_char *memory){
    size_t end = index + length;

    while(index <= end)
        printf("%d ", (int)memory[index++]);
    printf("| memc : %zu | value at memc : %hu | amount of loops : %zu | current loop condition : %zu | pc : %zu\n", memc, (__u_short)memory[memc], loops, loop_condition, pc);
}

int execute_bf(FILE* fptr){
    __u_char *memory = (__u_char *)calloc(MEMORY_SIZE, sizeof(__u_char));
    if(!memory){
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }
    size_t pc = 0, memc = 0;
    
    size_t loop_stack_capacity = 10, loops_amount = 0;
    size_t *loop_lengths = (size_t *)calloc(loop_stack_capacity, sizeof(size_t));
    if(!loop_lengths){
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
        #ifdef DEBUG
        if(commands_forward){
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

            print_memory(memory_index, DEBUG_MEMORY_DEPTH, memc, pc, loops_amount, invalid_loop, memory);
            scanf(" %c", &command);
        }

        while(getchar() != '\n');

        end:
        printf("\nExecuting command: %c | pc: %zu\n", c, pc);
        #endif

        if(loops_amount == loop_stack_capacity){
            loop_stack_capacity *= 2;
            size_t *new_data = (size_t *)realloc(loop_lengths, loop_stack_capacity * sizeof(size_t));
            loop_lengths = new_data;
        }

        if(c != OP_JMP_BCK && c != OP_JMP_FWD && invalid_loop){
            ++pc;
            continue;
        }

        switch (c)
        {
        case OP_INC_PC:
            memc = (memc + 1) % MEMORY_SIZE;
            break;
        case OP_DEC_PC:
            memc = (memc - 1 + MEMORY_SIZE) % MEMORY_SIZE;
            break;
        case OP_INC_VAL:
            memory[memc]++;
            break;
        case OP_DEC_VAL:
            memory[memc]--;
            break;
        case OP_IN:   
            __u_short in_val = get_input();

            while(getchar() != '\n');

            memory[memc] = in_val;

            break;
        case OP_OUT:
            #ifdef DEBUG
            printf("printed value: %d\n", memory[memc]);
            #else
            putchar(memory[memc]);
            #endif

            break;
        case OP_JMP_FWD:
            loop_lengths[loops_amount++] = pc;

            if(memory[memc] == 0){
                invalid_loop = 1;
                invalid_loop_start_index = loops_amount;
            }

            break;
        case OP_JMP_BCK:
            if(loops_amount == 0)
                return EXIT_FAILURE;

            if(loops_amount == invalid_loop_start_index){
                invalid_loop = 0;
                invalid_loop_start_index = 0;
            }
            
            size_t go_back = loop_lengths[--loops_amount];

            if(memory[memc] != 0){
                pc = go_back - 1;
                fseek(fptr, go_back, SEEK_SET);
            }
            
            break;
        }

        #ifdef DEBUG
        print_memory(memory_index, DEBUG_MEMORY_DEPTH, memc, pc, loops_amount, invalid_loop, memory);
        ++commands_since_start;
        #endif
        ++pc;
    }

    #ifdef DEBUG
    printf("\nprogram length : %zu\n", commands_since_start);
    #endif

    free(memory);
    free(loop_lengths);

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