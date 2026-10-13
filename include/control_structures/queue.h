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
bool queue_push_front(Queue* q, TaskControlBlock* data);

/**
 * @brief Ordena los elementos de la cola por el tiempo de llegada.
 * @details Aplica un algoritmo de ordenamiento estable (Bubble Sort sobre datos)
 *          evaluando 'arrival_time' ascendente. Si dos tareas arriban en el mismo 
 *          instante, se preserva estrictamente el orden del archivo de entrada.
 *
 * @param q Puntero a la cola a ordenar.
 */
void queue_sort(Queue* q);

/**
 * @brief Muestra por consola una tabla formateada con el estado y tareas en la cola.
 * @details Imprime el identificador, tiempo de arribo, prioridad y ráfagas de cada TCB.
 *          Diseñado para propósitos de depuración y auditoría.
 *
 * @param q Puntero constante a la cola a inspeccionar.
 */
void queue_print(const Queue* q);

#endif