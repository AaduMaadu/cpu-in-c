#include <stdio.h>
#include "disk.h"

#define MEMORY_SIZE 1024

int memory[MEMORY_SIZE][2];

int* mem_read(int addr)
{
    return memory[addr];
}

void mem_write(int addr, Data *data_ptr)
{
    printf("Writing to mem addr: %d\n", addr);
    memory[addr][0] = data_ptr->opcode;

    if (argExists) {
        memory[addr][1] = data_ptr->arg;
    }
}