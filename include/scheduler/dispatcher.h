#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "control_structures/task_control_block.h"

void dispatcher_release(TaskControlBlock* task);
#include "control_structures/queue.h"

void dispatcher_context_switch(TaskControlBlock** cpu_task, Queue* source_queue);
void dispatcher_quantum_expired(TaskControlBlock** cpu_task, Queue* target_queue, int current_tick);
void dispatcher_preempt(TaskControlBlock** cpu_task, Queue* target_queue, int current_tick);
void dispatcher_finish_task(TaskControlBlock** cpu_task, int current_tick);

#endif
