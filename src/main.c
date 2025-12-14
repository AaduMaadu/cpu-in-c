#include <stdio.h>
#include "cpu.h"
#include "disk.h"
#include "memory.h"
#include "scheduler.h"
#include "smm.h"

int main()
{   
    char filename[64] = "program_list_invalid_allocation.txt";
    int cycle_count = 1;
    int ready_queue_status = 1;

    add_hole(0, 1024); // Add hole by default to entire main memory size
    load_programs(filename);
    //load_prog("loop50.txt", 19);

    // Load the first process into CPU before starting clock cycles
    cpu = head->data.cpu;
    printf("Initial process loaded: %d (base=%d)\n", head->data.p_Id, cpu.base);

    /* Loop till program exits 
    ** While ready_queue is not empty, keep looping */ 
    while(ready_queue_status != 0)
    {
        int p_status = clock_cycle();
        ready_queue_status = schedule(cycle_count++, p_status);
    } 

    // Print first 20 memory locations
    // for (int i = 0; i < 20; i++) {
    //     int *memory_data = mem_read(i);
    //     printf("Memory location %d : [%d, %d]\n", i, memory_data[0], memory_data[1]);
    // }

    printf("Hole count: %d\n", holeCount);
    bypass_validation = true;
    int *memory_data = mem_read(30);
    printf("Memory location %d : [%d, %d]\n", 30, memory_data[0], memory_data[1]);
    *memory_data = mem_read(150);
    printf("Memory location %d : [%d, %d]\n", 150, memory_data[0], memory_data[1]);
    *memory_data = mem_read(230);
    printf("Memory location %d : [%d, %d]\n", 230, memory_data[0], memory_data[1]);

    return 0;
}