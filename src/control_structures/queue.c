#include <stdio.h>
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

bool queue_push_front(Queue* q, TaskControlBlock* data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        return false;
    }
    newNode->data = data;

    if (queue_empty(q)) {
        newNode->next = NULL;
        q->front = q->back = newNode;
    } else {
        newNode->next = q->front;
        q->front = newNode;
    }
    q->count++;
    return true;
}

void queue_sort(Queue* q) { // Ordenamiento Bubble Sort.
    if (queue_empty(q) || q->front->next == NULL) return;
    
    bool swapped;
    Node* current;
    Node* last_ptr = NULL;

    do {
        swapped = false;
        current = q->front;

        while (current->next != last_ptr) {
            // evaluamos el tiempo de llegada: si dos procesos llegan en el mismo Tg, no se intercambian,
            // conservando así su orden de aparición en el archivo de texto.
            if (current->data->arrival_time > current->next->data->arrival_time) {
                
                TaskControlBlock* temp_data = current->data;
                current->data = current->next->data;
                current->next->data = temp_data;
                
                swapped = true;
            }
            current = current->next;
        }
        // el elemento más grande ya "burbujeó" hasta el final, por lo que no es necesario revisarlo.
        last_ptr = current; 
        
    } while (swapped);
}

void queue_print(const Queue* q) {
    if (queue_empty(q)) {
        printf("Queue is empty.\n");
        return;
    }

    printf("\n==================== QUEUE STATE ====================\n");
    printf("Total tasks in queue: %zu\n", q->count);
    printf("----------------------------------------------------------------------\n");
    printf(" Pos | Sentinel ID | Arrival Time (Tg) | Queue Priority | Total Burst\n");
    printf("----------------------------------------------------------------------\n");

    Node* current = q->front;
    int index = 1;

    while (current != NULL) {
        TaskControlBlock* tcb = current->data;
        printf(" %02d  | %-12d | %-14d | %-16d | %-12d\n",
               index++,
               tcb->id,
               tcb->arrival_time,
               tcb->current_priority,
               tcb->total_burst);
        current = current->next;
    }

    printf("======================================================================\n\n");
}