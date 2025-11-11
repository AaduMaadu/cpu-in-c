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
    // ready queue is empty, nothing to schedule
    if (head == NULL) return 0;

    // If the running process exited, remove it and load the next process
    if (process_status == 0) 
    {
        deleteHead();
        // Terminate if ready queue empty
        if (head == NULL) return 0;

        context_switch(head->data.cpu);
        printf("Loaded process ID: %d\n", head->data.p_Id);
    }
    // Switch when quantum expires (except when there is only 1 process in the queue)
    else if (!(cycle_num % TIME_QUANTUM) && head->next != NULL) 
    {
        next_process();
        CPU_reg_t cpu_old = context_switch(head->data.cpu); 

        // Go to the end of the linked list
        Node *last_ptr = head;
        while (last_ptr->next != NULL) 
        {
            last_ptr = last_ptr->next;
        }
        // store outgoing CPU regs into the moved node
        last_ptr->data.cpu = cpu_old;

        printf("Switched to process ID: %d\n", head->data.p_Id);
    }

    return 1;
}

// Move head node to back of the queue without allocating/freeing nodes
void next_process()
{
    if (head == NULL || head->next == NULL) return; // zero or one node -> nothing to do

    Node *old = head;
    head = head->next;
    old->next = NULL;

    // append old head to tail
    Node *last_ptr = head;
    while (last_ptr->next != NULL) 
    {
        last_ptr = last_ptr->next;
    }
    last_ptr->next = old;
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
    // Allocate node
    Node *new_node = malloc(sizeof(Node));
    if (!new_node) 
    {
        perror("malloc");
        return;
    }

    // Fill data and mark next NULL
    new_node->data = new_data;
    new_node->next = NULL;

    // If list is empty, new node becomes head
    if (head == NULL) 
    {
        head = new_node;
        return;
    }

    // Otherwise append new node to tail
    Node *last_ptr = head;
    while (last_ptr->next != NULL) 
    {
        last_ptr = last_ptr->next;
    }
    last_ptr->next = new_node;
}

void deleteHead()
{
    if (head == NULL) return;
    Node *temp_ptr = head;
    head = head->next; // Changed head
    free(temp_ptr);
}