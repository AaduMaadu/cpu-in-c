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

/* Global ready-queue head pointer */
extern Node *head;

int schedule(int cycle_num, int process_status);
void next_process(void);
void new_process(int base, int size);

/* Linked-list helpers */
void appendNode(PCB_t new_data);
void deleteHead(void);

#endif // scheduler.h