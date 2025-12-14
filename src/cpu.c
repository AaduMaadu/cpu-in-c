#include <stdio.h>
#include <stdbool.h>
#include "cpu.h"
#include "memory.h"
#include "scheduler.h"

// Define the CPU register
CPU_reg_t cpu = {0};
bool EXIT_FLAG = false;

// Swap old cpu register values with new cpu register values
CPU_reg_t context_switch(CPU_reg_t new_vals) 
{
    CPU_reg_t cpu_old = cpu;
    cpu = new_vals;

    return cpu_old;
}

void fetch_instruction(int addr)
{
    printf("Fetching from instruction addr %d\n", addr);
    int *instruction_data = mem_read(addr);

    cpu.IR0 = instruction_data[0];

    if (instruction_data[1] == 0) {
        cpu.IR1 = 0;
    }
    cpu.IR1 = instruction_data[1];
    
}

/*
exit = 0
load_const = 1
move_from_mbr = 2
move_from_mar = 3
move_to_mbr = 4
move_to_mar = 5
load_at_addr = 6
write_at_addr = 7
add = 8
multiply = 9
and = 10
or = 11
ifgo = 12
sleep = 13
*/

void execute_instruction()
{
    printf("Executing opcode: %d\n", cpu.IR0);
    switch(cpu.IR0) {
        case 0:
            EXIT_FLAG = true;
            break;
        case 1:
            cpu.AC = cpu.IR1;
            break;
        case 2:
            cpu.AC = cpu.MBR;
            break;
        case 3:
            cpu.AC = cpu.MAR;
            break;
        case 4:
            cpu.MBR = cpu.AC;
            break;
        case 5:
            cpu.MAR = cpu.AC;
            break;
        case 6:
            cpu.MBR = *mem_read(cpu.MAR);
            break;
        case 7:
        {
            Data temp_data = {cpu.MBR, 0};
            mem_write(cpu.MAR, &temp_data);
        }
            break;
        case 8:
            cpu.AC = cpu.AC + cpu.MBR;
            break;
        case 9:
            cpu.AC = cpu.AC * cpu.MBR;
            break;
        case 10:
            cpu.AC = cpu.AC && cpu.MBR; // Logical AND
            break;
        case 11:
            cpu.AC = cpu.AC || cpu.MBR; // Logical OR
            break;
        case 12:
            if (cpu.AC != 0) {
                // Jump to the address specified in IR1 (-1 because PC will be incremented)
                cpu.PC = cpu.IR1 - 1; 
            }
            break;
        case 13:
            // Do nothing
            break;
        default:
            printf("ERROR: Invalid Opcode Found!\n");
    }
    //printf("AC Register contents: %d\n", cpu.AC);
    //printf("MBR contents: %d\n", cpu.MBR);
    //printf("MAR contents: %d\n", cpu.MAR);
}

int mem_address(int I_addr)
{
    return (cpu.base + I_addr);
}

int clock_cycle()
{
    fetch_instruction(mem_address(cpu.PC));
    execute_instruction();
    cpu.PC = cpu.PC + 1;

    if (EXIT_FLAG) {
        EXIT_FLAG = false; // reset exit flag status
        return 0;
    }

    return 1;
}