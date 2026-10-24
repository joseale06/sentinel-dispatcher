#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include "control_structures/queue.h"
#include "control_structures/task_control_block.h"

bool scheduler_init(Queue* temporal_queue);
void scheduler_destroy(void);
void scheduler_on_tick(int tick);
TaskControlBlock* scheduler_dispatch(void);
TaskControlBlock* scheduler_current_task(void);
void scheduler_finish_cpu_tick(int tick);
void scheduler_update_waiting_metrics(void);
bool scheduler_has_finished(void);

#endif