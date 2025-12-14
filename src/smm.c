#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "smm.h"

#define ALLOCATION_TABLE_SIZE 256

int holeCount = 0;
int allocation_table[ALLOCATION_TABLE_SIZE][3];
static HoleNode *head = NULL;

//  allocation table stores the PID, the base address, and the size of the process
int allocate(int pid, int size) 
{
    int base_addr = find_hole(size);
    // No viable hole is found
    if (base_addr == -1) return 0;

    HoleNode *ptr = head;
    bool flag = true;

    while (ptr->next != NULL && flag)
    {
        if (base_addr == ptr->data.begAddr)
        {
            flag = false;
            if (ptr->data.holeSize - size == 0) //  If the entire hole is lost, then it should be removed from the linked list
            {
                remove_hole(base_addr);
            }
            else { //  update the base address and size of the original hole to reflect the memory that has been lost
                ptr->data.holeSize = ptr->data.holeSize - size;
                ptr->data.begAddr = base_addr + size;
            }
        }
        ptr = ptr->next;
    }

    // Find allocation table row with size zero
    for (int i = 0; i < ALLOCATION_TABLE_SIZE; i++)
    {
        if (allocation_table[i][2] == 0)
        {
            allocation_table[i][0] = pid;
            allocation_table[i][1] = base_addr;
            allocation_table[i][2] = size;

            return 1;
        }
    }

    return 0;
}

void deallocate(int pid)
{

}

void add_hole(int base, int size)
{
    // Add hole by default to entire main memory size
    if (head == NULL)
    {
        head->data.begAddr = base;
        head->data.holeSize = size;
    }
}

void remove_hole(int base)
{
    HoleNode *ptr = head;
    HoleNode *prev = NULL;

    // If there are no holes
    if (ptr == NULL) return;

    // If head matches the base address
    if (ptr->data.begAddr == base)
    {
        HoleNode *temp_ptr = head;
        head = head->next; // Changed head
        free(temp_ptr);
        return;
    }

    // Search for hole to delete in linked list
    while (ptr->next != NULL)
    {
        prev = ptr;
        ptr = ptr->next;

        if (base == ptr->data.begAddr)
        {
            // Delete the hole
            prev->next = ptr->next;
            free(ptr);
        }
    }
}

void merge_holes()
{

}

// Uses first-fit algorithm to find holes
int find_hole(int size)
{
    HoleNode *ptr = head;
    bool holeFound = false;

    //If only head (no process)
    if (head->next == NULL)
    {
        if (size <= ptr->data.holeSize)
        {
            holeFound = true;
        }
    }

    while (ptr->next != NULL)
    {
        ptr = ptr->next;

        if (size <= ptr->data.holeSize)
        {
            holeFound = true;
            break;
        }
    } 

    if (!holeFound) return -1;
    
    return ptr->data.begAddr;
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