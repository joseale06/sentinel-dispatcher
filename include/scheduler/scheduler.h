#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include "control_structures/queue.h"
#include "control_structures/task_control_block.h"

bool scheduler_init(Queue* temporal_queue);
void scheduler_destroy(void);
void scheduler_on_tick(int tick);

// FUNCIONES DE EVALUACIÓN DE ESTADO.
TaskControlBlock** scheduler_get_active_task_ptr(void);
TaskControlBlock* scheduler_current_task(void);
Queue* scheduler_get_queue(Priority p);
Queue* scheduler_select_highest_priority_queue(void);

void scheduler_update_waiting_metrics(void);
bool scheduler_has_finished(void);

#endif