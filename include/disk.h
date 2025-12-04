#ifndef DISK_H
#define DISK_H

//#include <stdio.h>

typedef struct {
    int opcode;
    int arg;
} Data;

extern Data data;
/* `argExists` is internal to the parser implementation and should not be exposed here. */

/* Fixed prototypes: use valid parameter syntax and terminate with semicolons */
void load_programs(char[]);
void load_prog(char[], int);
Data* translate(char *);

#endif // disk.h