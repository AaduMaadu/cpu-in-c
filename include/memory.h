#ifndef MEMORY_H
#define MEMORY_H

#include "disk.h"
#include <stdbool.h>

// Function prototypes
int* mem_read(int addr);
void mem_write(int addr, Data *data_ptr);

extern bool bypass_validation;

#endif // MEMORY_H
