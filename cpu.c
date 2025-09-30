#include <stdio.h>
#include "cpu.h"
#include "memory.h"

// Define the CPU register structure
CPU_reg cpu;

__uint8_t EXIT_FLAG = 0;

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
igo = 12
sleep = 13
*/

void execute_instruction()
{
    printf("Executing opcode: %d\n", cpu.IR0);
    switch(cpu.IR0) {
        case 0:
            EXIT_FLAG = 1;
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
            cpu.AC = cpu.AC & cpu.MBR;
            break;
        case 11:
            cpu.AC = cpu.AC | cpu.MBR;
            break;
        case 12:
            if (cpu.AC != 0) {
                cpu.PC = cpu.IR1 - 1; // Jump to the address specified in IR1, -1 because PC will be incremented
            }
            break;
        case 13:
            // Do nothing
            break;
        default:
            printf("ERROR: Invalid Opcode Found!\n");
    }
    printf("AC Register contents: %d\n", cpu.AC);
    printf("MBR contents: %d\n", cpu.MBR);
    printf("MAR contents: %d\n", cpu.MAR);
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

    if (EXIT_FLAG)
        return 0;

    return 1;
}