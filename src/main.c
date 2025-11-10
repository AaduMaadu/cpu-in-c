#include <stdio.h>
#include "cpu.h"
#include "disk.h"
#include "memory.h"
#include "scheduler.h"

char filename[32] = "program_list.txt";

int main()
{   
    int cycle_count = 0;

    //load_prog(file, cpu.base);
    load_programs(filename);

    // Loop till program exits
    // while ready_queue is not empty, keep looping
    while(head != NULL)
    {
        int p_status = clock_cycle();
        schedule(cycle_count++, p_status);
    } 

    // Print first 20 memory locations
    for (int i = 0; i < 20; i++) {
        int *memory_data = mem_read(i);
        printf("Memory location %d : [%d, %d]\n", i, memory_data[0], memory_data[1]);
    }

    return 0;
}