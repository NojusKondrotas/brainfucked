#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 30000

int execute_bf(FILE* fptr){
    char *memory = (char *)calloc(MEMORY_SIZE, sizeof(char));
    if(!memory){
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }
    size_t pc = 0, memc = 0;
    
    size_t capacity = 10, size = 0;
    size_t *loop_counters = (size_t *)calloc(capacity, sizeof(size_t));
    if(!loop_counters){
        fprintf(stderr, "Failure allocating memory\n");
        return EXIT_FAILURE;
    }

    char c;
    while((c = fgetc(fptr)) != EOF){
        if(size == capacity){
            capacity *= 2;
            size_t *new_data = (size_t *)realloc(loop_counters, capacity * sizeof(size_t));
            loop_counters = new_data;
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
            memory[memc] = getchar();
            break;
        case '.':
            putchar(memory[memc]);
            break;
        case '[':
            loop_counters[size++] = pc;
            break;
        case ']':
            if(size == 0)
                return EXIT_FAILURE;
            
            size_t go_back = loop_counters[--size];
            loop_counters[size] = 0;
            

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