#ifndef DISK_H
#define DISK_H

typedef struct {
    int opcode;
    int arg;
} Data;

extern Data data;
extern int argExists;

void load_prog(FILE *, int);
Data* translate(char *);

#endif // disk.h