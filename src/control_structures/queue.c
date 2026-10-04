#include <stdlib.h>
#include "control_structures/task_control_block.h"
#include "control_structures/queue.h"

void queue_init(Queue* q) { //eq. al constructor.
    q->front = NULL;
    q->back = NULL;
    q->count = 0;
}

bool queue_enqueue(Queue* q, TaskControlBlock* data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        return false;
    }
    newNode->data = data;
    newNode->next = NULL;

    if (queue_empty(q)) {
        q->front = q->back = newNode;
    } else {
        q->back->next = newNode;
        q->back = newNode;
    }
    q->count++;
    return true;
}

bool queue_empty(const Queue* q) { return (q->front == NULL); }

TaskControlBlock* queue_dequeue(Queue* q) {
    if (queue_empty(q)) return NULL;

    Node* auxPtr = q->front;
    TaskControlBlock* extractedData = auxPtr->data;
    
    q->front = q->front->next;
    free(auxPtr);
    q->count--;

    if (queue_empty(q)) q->back = NULL;

    return extractedData;
}

Node* queue_front(const Queue* q) { 
    if (queue_empty(q)) { return NULL; }

    return q->front; 
}

size_t queue_size(const Queue* q) { return q->count; }

void queue_destroy(Queue* q) {
    while (!queue_empty(q)) queue_dequeue(q);
}

void queue_sort(Queue* q) {
    //
}
