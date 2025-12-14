#ifndef SMM_H
#define SMM_H

typedef struct {
    int begAddr;
    int holeSize;
} HoleList_t;

// Memory hole node
typedef struct HoleNode {
    HoleList_t data;
    struct HoleNode *next;
} HoleNode;

int allocate(int pid, int size);
void deallocate(int pid);
void add_hole(int base, int size);
void remove_hole(int base);
void merge_holes();
int find_hole(int size);
int get_base_adddress(int pid);
int find_empty_row();
int is_allowed_address(int pid, int addr);


#endif // smm.h