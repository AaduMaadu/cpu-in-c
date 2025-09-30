#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "disk.h"

// Define the global data structure
Data data;

int argExists = 0;

void load_prog(FILE *fname, int addr)
{
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), fname) != NULL) 
    {
        if (buffer[0] == '/')
            continue;

        mem_write(addr++, translate(buffer));
    }
}

Data* translate(char *instruction)
{
    char *ptr = instruction;
    char opcode[32], arg[32];
    
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
    if (*ptr == ' ') {
        ptr++; //skip space
        i = 0;
        while (*ptr != '\0' && *ptr != '\n' && i < 31) 
        {
            arg[i++] = *ptr++;
        }
        arg[i] = '\0';

        data.arg = atoi(arg);
        argExists = 1;
    } else {
        argExists = 0;
        data.arg = 0;
    }

    printf("Parsing: opcode='%s', arg='%s'\n", opcode, arg);

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