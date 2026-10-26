#include <stdio.h>
#include <stdlib.h>
#include "scheduler/dispatcher.h"
#include "queue_policies/rr_policy.h"

void dispatcher_context_switch(TaskControlBlock** cpu_task, Queue* source_queue) {
    if (source_queue == NULL || queue_empty(source_queue)) return;
    
    *cpu_task = queue_dequeue(source_queue);
    (*cpu_task)->current_status = RUNNING;
}

void dispatcher_quantum_expired(TaskControlBlock** cpu_task, Queue* target_queue, int current_tick) {
    if (*cpu_task == NULL) return;

    printf("[T-%04d] QUANTUM_EXPIRED: Sentinel %d yields CPU via RR rotation (Q=%d)\n", 
        current_tick, 
        (*cpu_task)->id,
        round_robin_quantum((*cpu_task)->current_priority));
    
    (*cpu_task)->current_status = READY;
    (*cpu_task)->used_quantum = 0;
    queue_enqueue(target_queue, *cpu_task);
    *cpu_task = NULL;
}

void dispatcher_preempt(TaskControlBlock** cpu_task, Queue* target_queue, int current_tick) {
    if (*cpu_task == NULL) return;

    printf("[T-%04d] PREEMPTED: Sentinel %d yields CPU to higher priority\n", current_tick, (*cpu_task)->id);
    
    (*cpu_task)->current_status = READY;
    // se inserta a la cabeza conservando su remaining_burst y used_quantum.
    queue_push_front(target_queue, *cpu_task);
    *cpu_task = NULL;
}

void dispatcher_finish_task(TaskControlBlock** cpu_task, int current_tick) {
    if (*cpu_task == NULL) return;

    (*cpu_task)->current_status = FINISHED;
    (*cpu_task)->return_time = current_tick - (*cpu_task)->arrival_time;
    
    printf("[T-%04d] COMPLETED: Sentinel ID: %d -> Turnaround Time: %d ticks\n",
           current_tick, (*cpu_task)->id, (*cpu_task)->return_time);
           
    free(*cpu_task);
    *cpu_task = NULL;
}