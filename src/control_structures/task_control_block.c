#include <stdlib.h>
#include "control_structures/task_control_block.h"

TaskControlBlock* TCB_create(int id, int arrival_time, Priority priority, int total_burst) {
    TaskControlBlock *tcb = (TaskControlBlock*)malloc(sizeof(TaskControlBlock));
    if (tcb == NULL) return NULL;
    
    tcb->id = id;
    tcb->arrival_time = arrival_time;
    tcb->current_priority = priority;
    tcb->total_burst = total_burst;
    
    // Inicialización del estado operativo y métricas.
    tcb->remaining_burst = total_burst;
    tcb->current_status = NEW;
    tcb->used_quantum = 0;
    tcb->wait_time = 0;
    tcb->return_time = 0;

    return tcb;
}