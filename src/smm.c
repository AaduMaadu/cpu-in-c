#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "smm.h"

#define ALLOCATION_TABLE_SIZE 256

int holeCount = 0;
int allocation_table[ALLOCATION_TABLE_SIZE][3];
/* ALTERNATE OPTION: Create fake head in order to attach subsequent hole nodes in the linked list */
static HoleNode *head = NULL; 
// head->data.begAddr = -1;
// head->data.holeSize = -1;


//  allocation table stores the PID, the base address, and the size of the process
int allocate(int pid, int size) 
{
    int base_addr = find_hole(size);
    // No viable hole is found
    if (base_addr == -1) return 0;

    int row = find_empty_row();
    // If no empty rows are found
    if (row == -1) return 0;

    HoleNode *ptr = head;
    bool flag = false;

    if (head->next == NULL)
    {
        flag = true;
    }

    while (ptr->next != NULL)
    {
        if (ptr->data.begAddr == base_addr)
        {
            flag = true;
        }
        ptr = ptr->next;
    }

    if (flag)
    {
        // If the entire hole is lost, then it should be removed from the linked list
        if (ptr->data.holeSize - size == 0) 
        {
            remove_hole(base_addr);
        }
        else { // update the base address and size of the original hole to reflect the memory that has been lost
            ptr->data.holeSize = ptr->data.holeSize - size;
            ptr->data.begAddr = base_addr + size;
        }
    }
    else {
        printf("failed to match base addr during allocation\n");
        return 0;
    }

    allocation_table[row][0] = pid;
    allocation_table[row][1] = base_addr;
    allocation_table[row][2] = size;

    return 1;
}

void deallocate(int pid)
{
    printf("Deallocating process %d memory...\n", pid);
    int base_addr = get_base_adddress(pid);
    int size = get_size(pid);

    // Add new hole from the deallocated process
    add_hole(base_addr, size);

    // Set size of corresponding row in the allocation table to 0
    allocation_table[get_row(pid)][2] = 0;

}

void add_hole(int base, int size)
{
    int lastAddr = base + size;
    HoleNode *new_node = malloc(sizeof(HoleNode));
    if (!new_node) 
    {
        perror("malloc");
        return;
    }

    new_node->data.begAddr = base;
    new_node->data.holeSize = size;
    new_node->next = NULL;

    // Head is first set as a hole with the maximum contiguous block of memory
    if (head == NULL)
    {
        head = new_node;
        holeCount++;
        printf("Added head for HoleList with base=%d, size=%d\n", base, size);
        return;
    }

    HoleNode *cur_ptr = head;
    HoleNode *prev_ptr = NULL;

    // If only head present (single hole)
    if (cur_ptr->next == NULL)
    {
        // If new hole would be adjacent to current hole
        // Expand the existing hole instead of adding a new hole
        if (lastAddr == cur_ptr->data.begAddr)
        {
            cur_ptr->data.begAddr = base;
            free(new_node);
            printf("Expanded hole with base=%d, size=%d\n", base, size);
        }
        else { // Otheriwse append new hole to head
            new_node->next = head;
            head = new_node;
            holeCount++;
            printf("Added hole with base=%d, size=%d\n", base, size);
        }
        return;   
    }

    while (cur_ptr->next != NULL) 
    {
        prev_ptr = cur_ptr;
        cur_ptr = cur_ptr->next;
        
        // Insert new hole between two holes
        if ((base > prev_ptr->data.begAddr) && (lastAddr < cur_ptr->data.begAddr))
        {
            new_node->next = cur_ptr;
            prev_ptr->next = new_node;
            holeCount++;
            printf("Inserted hole with base=%d, size=%d\n", base, size);

            // Check for adjacent holes to be merged
            merge_holes();
            return;
        }
    }
    
}

void remove_hole(int base)
{
    HoleNode *temp_ptr = head;
    HoleNode *prev = NULL;

    // If there are no holes
    if (temp_ptr == NULL) return;

    // If head matches the base address
    if (temp_ptr->data.begAddr == base)
    {
        head = head->next; // Changed head
        free(temp_ptr);
        holeCount--;
        printf("Removed hole with base=%d\n", base);
        return;
    }

    // Search for hole to delete in linked list
    while (temp_ptr->next != NULL)
    {
        prev = temp_ptr;
        temp_ptr = temp_ptr->next;

        if (temp_ptr->data.begAddr == base)
        {
            // Delete the hole
            prev->next = temp_ptr->next;
            free(temp_ptr);
            holeCount--;
            printf("Removed hole with base=%d\n", base);
            return;
        }
    }
}

void merge_holes()
{
    HoleNode *cur_ptr = head;
    HoleNode *prev_ptr = NULL;

    while (cur_ptr->next != NULL)
    {
        prev_ptr = cur_ptr;
        cur_ptr = cur_ptr->next;
        int lastAddr = prev_ptr->data.begAddr + prev_ptr->data.holeSize;

        // If two holes are next to each other, merge into one
        if (((lastAddr + 1) == cur_ptr->data.begAddr) || (lastAddr == cur_ptr->data.begAddr) || (lastAddr == (cur_ptr->data.begAddr - 1)))
        {
            prev_ptr->data.holeSize = prev_ptr->data.holeSize + cur_ptr->data.holeSize;
            prev_ptr->next = cur_ptr->next;
            free(cur_ptr);
            holeCount--;
            printf("Merged hole with lastAddr=%d\n", lastAddr);
            return;
        }
    }
}

// Uses first-fit algorithm to find holes
int find_hole(int size)
{
    HoleNode *ptr = head;
    bool holeFound = false;

    // If only head present
    if (head->next == NULL)
    {
        if (size <= ptr->data.holeSize)
        {
            holeFound = true;
        }
    }
    // otherwise search through list for a hole
    while (ptr->next != NULL && !holeFound)
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
    int base_addr = 0;
    for (int i = 0; i < ALLOCATION_TABLE_SIZE; i++)
    {
        if (allocation_table[i][0] == pid)
        {
            base_addr = allocation_table[i][1];
            break;
        }
    }
    return base_addr;
}

int find_empty_row()
{
    bool empty_row = false;
    int row = 0;

    for (int i = 0; i < ALLOCATION_TABLE_SIZE; i++)
    {
        // Find allocation table row with size zero
        if (allocation_table[i][2] == 0)
        {
            empty_row = true;
            row = i;
            break;
        }
    }
    if (!empty_row) return -1;
    printf("row num=%d\n", row);
    return row;
}

int is_allowed_address(int pid, int addr)
{
    int size = get_size(pid);
    int base_addr = get_base_adddress(pid);
    int lastAddr = size + base_addr;

    //printf("size=%d, base_addr=%d, curAddr=%d\n", size, base_addr, addr);

    // Return 0 if out of bounds
    if (addr < base_addr || addr > lastAddr)
    {
        return 0;
    }
    return 1;
}

int get_size(int pid)
{
    int size = -1;
    for (int i = 0; i < ALLOCATION_TABLE_SIZE; i++)
    {
        if (allocation_table[i][0] == pid)
        {
            size = allocation_table[i][2];
            break;
        }
    }
    if (size == -1)
    {
        printf("failed to find valid size!\n");
    }
    return size;
}

int get_row(int pid)
{
    int row = -1;
    for (int i = 0; i < ALLOCATION_TABLE_SIZE; i++)
    {
        if (allocation_table[i][0] == pid)
        {
            row = i;
            break;
        }
    }
    if (row == -1)
    {
        printf("failed to find valid row!\n");
    }
    return row;
}