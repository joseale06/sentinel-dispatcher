#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>
#include "task_control_block.h"

// declaración anticipada (forward declaration) para evitar problemas con espacio de nombres.
typedef struct Node Node;

struct Node { // estructura autorreferenciada, se emplea forward declaration.
    TaskControlBlock* data;
    struct Node* next;
};

typedef struct Queue {
    Node* front;
    Node* back;
    size_t count;
} Queue;

void queue_init(Queue* q);
bool queue_enqueue(Queue* q, TaskControlBlock* data);
bool queue_empty(const Queue* q);    
TaskControlBlock* queue_dequeue(Queue* q);         
Node* queue_front(const Queue* q);
size_t queue_size(const Queue* q);
void queue_destroy(Queue* q);
void queue_sort(Queue* q);

#endif