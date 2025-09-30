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
} CPU_reg;

extern CPU_reg cpu;

void fetch_instruction(int);
void execute_instruction();
int clock_cycle();

#endif // CPU_H