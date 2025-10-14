#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "cpu.h"

typedef struct {
    int p_Id;
    int p_size;
    CPU_reg cpu;
} PCB;

typedef struct {
    PCB data;
    struct Node *next;
} Node;

int schedule(int cycle_num, int process_status);
void next_process();
void new_process(int base, int size);

#endif // scheduler.h