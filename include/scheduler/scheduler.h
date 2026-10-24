#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include "control_structures/queue.h"
#include "control_structures/task_control_block.h"

bool scheduler_init(Queue* temporal_queue);
void scheduler_destroy(void);
void scheduler_on_tick(int tick);
void scheduler_execute_tick(int tick);
void scheduler_set_current_task(TaskControlBlock* task);
bool scheduler_has_finished(void);

#endif