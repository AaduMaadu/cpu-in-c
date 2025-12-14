#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scheduler.h"
#include "cpu.h"
#include "smm.h"

#define PROCESS_TABLE_SIZE 1024

const int TIME_QUANTUM = 10; // default to 10 clock cycles
PCB_t process_table[PROCESS_TABLE_SIZE];
int pt_index = 0;
Node* head = NULL;
bool initialized = false;

int schedule(int cycle_num, int process_status)
{
    // ready queue is empty, nothing to schedule
    if (head == NULL) return 0;

    // If the running process exited, remove it and load the next process
    if (process_status == 0) 
    {
        printf("Process status: %d\n", process_status);
        printf("Removing process ID: %d\n", head->data.p_Id);
        remove_process(head->data.p_Id);
        // Terminate if ready queue empty
        if (head == NULL) return 0;

        context_switch(head->data.cpu);
        printf("Loaded process ID: %d\n", head->data.p_Id);
    }
    // Switch when quantum expires (except when there is only 1 process in the queue)
    else if ((cycle_num % TIME_QUANTUM == 0) && head->next != NULL) 
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

    //printf("New process added: pid=%d base=%d size=%d\n", pcb_new.p_Id, base, size);
}

void remove_process(int pid)
{
    printf("Removing process %d from ready queue...\n", pid);
    deallocate(pid);

    Node *ptr = head;
    Node *prev = NULL;

    // If ready queue is empty
    if (ptr == NULL) return;

    // If head matches the pid
    if (ptr->data.p_Id == pid)
    {
        deleteHead();
        return;
    }

    // Search for process in ready queue
    while (ptr->next != NULL)
    {
        prev = ptr;
        ptr = ptr->next;

        if (ptr->data.p_Id == pid)
        {
            // Delete process from ready queue
            prev->next = ptr->next;
            free(ptr);
            return;
        }
    }
}

// Get PID of currently running process
int get_pid()
{
    if (head == NULL) return -1;
    return head->data.p_Id;
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
    Node *ptr = head;
    while (ptr->next != NULL) 
    {
        ptr = ptr->next;
    }
    ptr->next = new_node;

    printf("Appended node\n");
}

void deleteHead()
{
    if (head == NULL) return;
    Node *temp_ptr = head;
    head = head->next; // Changed head
    free(temp_ptr);
}