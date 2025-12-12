#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "smm.h"

#define COLUMNS 256
#define ROWS 3

int holeCount = 0;
int allocation_table[ROWS][COLUMNS];
HoleNode *head = NULL;

//  allocation table stores the PID, the base address, and the size of the process
int allocate(int pid, int size) 
{
    int base_addr = find_hole(size);
    // No viable hole is found
    if (base_addr == -1) return 0;

    // Find allocation table row with size zero
    for (int i = 0; i < ROWS; i++)
    {
        if (allocation_table[i][2] == 0)
        {
            allocation_table[i][0] = pid;
            allocation_table[i][1] = base_addr;
            allocation_table[i][2] = size;
        }
    }

    return 1;
}

void deallocate(int pid)
{

}

void add_hole(int base, int size)
{

}

void remove_hole(int base)
{

}

void merge_holes()
{

}

// Uses first-fit algorithm to find holes
int find_hole(int size)
{
    int hole = 0;

    return hole;
}

int get_base_adddress(int pid)
{

}

int find_empty_row()
{

}

int is_allowed_address(int pid, int addr)
{

}