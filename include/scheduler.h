#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "cpu.h"

typedef struct {
    int p_Id;
    int p_size;
    CPU_reg_t cpu;
} PCB_t;

// Ready queue node (named struct so it can reference itself)
typedef struct Node {
    PCB_t data;
    struct Node *next;
} Node;

extern Node *head; /* Global ready-queue head pointer */

int schedule(int cycle_num, int process_status);
void next_process(void);
void new_process(int base, int size);
int get_pid(void);
void remove_process(int pid);

/* Linked-list helpers */
void appendNode(PCB_t new_data);
void deleteHead(void);

#endif // scheduler.h