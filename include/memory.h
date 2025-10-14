#ifndef MEMORY_H
#define MEMORY_H

#include "disk.h"

// Function prototypes
int* mem_read(int addr);
void mem_write(int addr, Data *data_ptr);

#endif // MEMORY_H
