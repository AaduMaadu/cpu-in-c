#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "disk.h"
#include "scheduler.h"
#include "smm.h"

// Define the global data structure
Data data;
int argExists = 0;
int p_count = 0;

void load_programs(char fname[])
{
    char buffer[64];
    char memSize_str[16], filename[64];
    
    // Initialize arrays
    memset(memSize_str, 0, sizeof(memSize_str));
    memset(filename, 0, sizeof(filename));

    FILE* file = fopen(fname, "r");
    if (file == NULL) 
    {
        printf("Cannot open file '%s'\n", fname);
        exit(1);
    }
    printf("Successfully opened file %s\n", fname);

    while (fgets(buffer, sizeof(buffer), file) != NULL) 
    {
        char *ptr = buffer;

        /* Reset temporary buffers for each line to avoid leftover characters */
        memset(memSize_str, 0, sizeof(memSize_str));
        memset(filename, 0, sizeof(filename));

        // Extract memory position of program
        int i = 0;
        while (*ptr != ' ' && *ptr != '\r' && *ptr != '\n' && *ptr != '\0') 
        {
            memSize_str[i++] = *ptr++;
        }
        int memSize = atoi(memSize_str);

        // Skip spaces between size and filename
        while (*ptr == ' ') ptr++;

        // Extract program file name (trim CR/LF)
        i = 0;
        while (*ptr != '\n' && *ptr != '\r' && *ptr != '\0' && i < 31)
        {
            filename[i++] = *ptr++;
        }
        filename[i] = '\0';

        if (allocate(p_count, memSize))
        {
            load_prog(filename, memSize);
            p_count++;
        }
        else {
            printf("Process %d rejected by SMM!", p_count);
        }
    }
    fclose(file);
}

void load_prog(char fname[], int memSize)
{
    char buffer[64];
    int p_size = 0; // Holds the memory size of the actual isntructions of a process
    int base_addr = get_base_adddress(p_count);

    FILE* file = fopen(fname, "r");
    if (file == NULL) 
    {
        printf("Cannot open file %s\n", fname);
        exit(1);
    }
    printf("Successfully opened file %s\n", fname);

    // Add process to scheduler ready queue
    new_process(base_addr, memSize); 

    while (fgets(buffer, sizeof(buffer), file) != NULL) 
    {
        // Skip comments
        if (buffer[0] == '/')
            continue;

        mem_write(base_addr++, translate(buffer));
        p_size++;
    }
    
    fclose(file);
}

Data* translate(char *instruction)
{
    char *ptr = instruction;
    char opcode[32], arg[16];
    
    // Initialize arrays
    memset(opcode, 0, sizeof(opcode));
    memset(arg, 0, sizeof(arg));

    int i = 0;
    // Copy opcode until space, newline, or end
    while (*ptr != ' ' && *ptr != '\0' && *ptr != '\n' && i < 31) 
    {
        opcode[i++] = *ptr++;
    }
    opcode[i] = '\0';

    //If space is found, then arg exists
    if (*ptr == ' ') 
    {
        ptr++; //skip space
        i = 0;
        while (*ptr != '\0' && *ptr != '\n' && i < 31) 
        {
            arg[i++] = *ptr++;
        }
        arg[i] = '\0';

        data.arg = atoi(arg);
        argExists = 1;
    } else 
    {
        argExists = 0;
        data.arg = 0;
    }

    //printf("Parsing: opcode='%s', arg='%s'\n", opcode, arg);

    if (strcmp(opcode, "exit") == 0) {
        data.opcode = 0;
    } else if (strcmp(opcode, "load_const") == 0) {
        data.opcode = 1;
    } else if (strcmp(opcode, "move_from_mbr") == 0) {
        data.opcode = 2;
    } else if (strcmp(opcode, "move_from_mar") == 0) {
        data.opcode = 3;
    } else if (strcmp(opcode, "move_to_mbr") == 0) {
        data.opcode = 4;
    } else if (strcmp(opcode, "move_to_mar") == 0) {
        data.opcode = 5;
    } else if (strcmp(opcode, "load_at_addr") == 0) {
        data.opcode = 6;
    } else if (strcmp(opcode, "write_at_addr") == 0) {
        data.opcode = 7;
    } else if (strcmp(opcode, "add") == 0) {
        data.opcode = 8;
    } else if (strcmp(opcode, "multiply") == 0) {
        data.opcode = 9;
    } else if (strcmp(opcode, "and") == 0) {
        data.opcode = 10;
    } else if (strcmp(opcode, "or") == 0) {
        data.opcode = 11;
    } else if (strcmp(opcode, "ifgo") == 0) {
        data.opcode = 12;
    } else if (strcmp(opcode, "sleep") == 0) {
        data.opcode = 13;
    } else {
        data.opcode = -1; // Unknown opcode
    }

    return &data;
}