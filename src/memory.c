#include <stdio.h>
#include "disk.h"
#include "smm.h"
#include "scheduler.h"
#include "memory.h"

#define MEMORY_SIZE 1024

int memory[MEMORY_SIZE][2];
bool bypass_validation = false;

int* mem_read(int addr)
{
    if (!bypass_validation)
    {
        int pid = get_pid();
        // Check if read address is valid
        if (!is_allowed_address(pid, addr))
        {
            printf("Error: Process %d has an Illegal Memory Read Operation at addr %d\n", pid, addr);
            remove_process(pid); // Remove process from scheduler ready queue
            EXIT_FLAG = true; // Set exit flag to terminate process execution in CPU
            return;
        }
    }
    return memory[addr];
}

void mem_write(int addr, Data *data_ptr)
{
    if (addr > MEMORY_SIZE || addr < 0) 
    {
        printf("Error: Invalid memory address!");
        return;
    }
    int pid = 0;
    if (process_loading)
    {
        pid = p_count;
    }
    else {
        pid = get_pid();
    }
    // Check if write address is valid 
    if (!is_allowed_address(pid, addr)) 
    {
        printf("Error: Process %d has an Illegal Write Operation at addr %d\n", pid, addr);
        remove_process(pid); // Remove process from scheduler ready queue
        EXIT_FLAG = true; // Set exit flag to terminate process execution in CPU
        return;
    }
    printf("Writing to mem addr: %d\n", addr);
    memory[addr][0] = data_ptr->opcode;
    memory[addr][1] = data_ptr->arg;
}