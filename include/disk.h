#ifndef DISK_H
#define DISK_H

#include <stdbool.h>

typedef struct {
    int opcode;
    int arg;
} Data;

extern Data data;
extern bool process_loading;
extern int p_count;

/* Fixed prototypes: use valid parameter syntax and terminate with semicolons */
void load_programs(char[]);
void load_prog(char[], int);
Data* translate(char *);

#endif // disk.h