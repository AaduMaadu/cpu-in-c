#include <stdio.h>
#include "scheduler.h"

#define PROCESS_TABLE_SIZE 1024

int time_quantum = 10;
int process_table[PROCESS_TABLE_SIZE];
Node* head = NULL;

int schedule(int cycle_num, int process_status) 
{

}

void next_process() 
{

}

void new_process(int base, int size) 
{

}

// Given a reference (pointer to pointer) to 
// the head of a list and an int, appends a 
// new node at the end  
void appendNode(Node** head_ref, PCB new_data)
{
    // 1. Allocate node 
    Node* new_node = (Node*) malloc(sizeof(Node));

    // Used in step 5
    Node *last = *head_ref;  
 
    // 2. Put in the data  
    new_node->data = new_data;

    // 3. This new node is going to be the last node, so make next of it as NULL
    new_node->next = NULL;

    // 4. If the Linked List is empty, then make the new node as head 
    if (*head_ref == NULL)
    {
       *head_ref = new_node;
       return;
    }  
     
    // 5. Else traverse till the last node 
    while (last->next != NULL)
        last = last->next;
 
    // 6. Change the next of last node 
    last->next = new_node;  
}

void deleteHead(Node** head_ref)
{
    Node *temp = *head_ref;
    *head_ref = temp->next; // Changed head
     free(temp);

}