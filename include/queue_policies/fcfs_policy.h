#ifndef FCFS_POLICY_H
#define FCFS_POLICY_H

#include "control_structures/task_control_block.h"

void fcfs_execute_tick(TaskControlBlock* task);

#endif