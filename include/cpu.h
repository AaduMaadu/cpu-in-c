#ifndef CPU_H
#define CPU_H

typedef struct {
    int base;
    int PC;
    int IR0;
    int IR1;
    int AC;
    int MAR;
    int MBR;
} CPU_reg_t;

extern CPU_reg_t cpu;
extern int cycle_count;

CPU_reg_t context_switch(CPU_reg_t);
void fetch_instruction(int);
void execute_instruction();
int clock_cycle();

#endif // CPU_H