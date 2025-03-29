#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 30000

void print_memory(size_t index, size_t length, size_t memc, size_t pc, size_t loops, size_t loop_condition, unsigned short int *mem){
    size_t end = index + length;

    while(index <= end)
        printf("%hu ", mem[index++]);
    printf("| memc : %zu | value at memc : %hu | loops : %zu | loop condition : %zu | pc : %zu\n", memc, mem[memc], loops, loop_condition, pc);
}

int execute_bf(FILE* fptr){
    unsigned short int *memory = (unsigned short int *)calloc(MEMORY_SIZE, sizeof(unsigned short int));
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
    while((c = fgetc(fptr)) != EOF){
        if(loops == loop_stack_capacity){
            loop_stack_capacity *= 2;
            size_t *new_data = (size_t *)realloc(loop_counters, loop_stack_capacity * sizeof(size_t));
            loop_counters = new_data;
        }

        #if defined(DEBUG) && defined(DEBUG_MEMORY_DEPTH)
        print_memory(0, DEBUG_MEMORY_DEPTH, memc, pc, loops, invalid_loop, memory);
        #elif defined (DEBUG)
        print_memory(0, memc, memc, pc, loops, invalid_loop, memory);
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
            #ifdef DEBUG
            scanf("%hu", &memory[memc]);
            #else
            scanf("%hu", &memory[memc]);
            #endif

            break;
        case '.':
            #ifdef DEBUG
            printf("printed value: %hu\n", memory[memc]);
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
            loop_counters[loops] = 0;
            

            if(memory[memc] != 0){
                pc = go_back - 1;
                fseek(fptr, go_back, SEEK_SET);
            }
            
            break;
        }

        ++pc;
    }

    free(memory);
    free(loop_counters);

    return EXIT_SUCCESS;
}

int main(int argc, char **argv){
    FILE *fptr;

    if(argc < 2 || (fptr = fopen(argv[1], "r")) == NULL){
        fprintf(stderr, "File opening failure: %s\n", argv[0]);
        return EXIT_FAILURE;
    }

    int status = execute_bf(fptr);

    fclose(fptr);

    return status;
}