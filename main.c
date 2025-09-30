#include <stdio.h>
#include "cpu.h"
#include "disk.h"
#include "memory.h"

FILE* file;
char filename[64] = "program_if.txt";
char buffer[256];

int main()
{   
    file = fopen(filename, "r");

    if (file == NULL) {
        printf("Cannot open file '%s'\n", filename);
        return 1;
    }
    printf("Successfully opened file %s\n", filename);

    cpu.base = 4;
    cpu.PC = 0;

    load_prog(file, cpu.base);

    // Loop till program exits
    while (clock_cycle())
    ; 

    // Print first 20 memory locations
    for (int i = 0; i < 20; i++) {
        int *memory_data = mem_read(i);
        printf("Memory location %d : [%d, %d]\n", i, memory_data[0], memory_data[1]);
    }

    fclose(file);
    return 0;
}