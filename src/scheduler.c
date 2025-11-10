#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "cpu.h"

#define PROCESS_TABLE_SIZE 1024

const int TIME_QUANTUM = 10; // default to 10 clock cycles
PCB_t process_table[PROCESS_TABLE_SIZE];
int pt_index = 0;
Node* head = NULL;

int schedule(int cycle_num, int process_status) 
{
    // Time quantum expiration check
    if (cycle_num % TIME_QUANTUM == 0)
    {
        // Process has terminated -> remove from ready queue
        if (process_status == 0) {
            deleteHead();
        }
        // ready queue is empty
        if (head == NULL) return 0; 

        next_process();
        PCB_t PCB_cur = process_table[pt_index];
        context_switch(PCB_cur.cpu);
    }
    return 1;
}

// Move head node to back of the queue
void next_process() 
{
    appendNode(head->data);
    deleteHead();
}

void new_process(int base, int size) 
{
    PCB_t pcb_new;
    pcb_new.p_Id = pt_index;
    pcb_new.p_size = size;
    pcb_new.cpu = (CPU_reg_t){0}; // Use a compound literal to zero-initialize the CPU_reg_t
    pcb_new.cpu.base = base;
    process_table[pt_index] = pcb_new;
    pt_index++;

    /* Add process to the end of ready queue */
    appendNode(pcb_new);
}

// Given a reference (pointer to pointer) to 
// the head of a list and an int, appends a 
// new node at the end  
void appendNode(PCB_t new_data)
{
    // 1. Allocate node
    Node* new_node = malloc(sizeof(Node));
    if (!new_node) {
        perror("malloc");
        return;
    }

    // 2. Fill data and mark next NULL
    new_node->data = new_data;
    new_node->next = NULL;

    // 3. If list is empty, new node becomes head
    if (head == NULL) {
        head = new_node;
        return;
    }

    // 4. Otherwise find last and append
    Node *last = head;
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
}

void deleteHead()
{
    if (head == NULL) return;
    Node *temp = head;
    head = head->next; // Changed head
    free(temp);
}