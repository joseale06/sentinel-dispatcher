#include "queue_policies/fcfs_policy.h"

void fcfs_execute_tick(TaskControlBlock* task) {
    task->remaining_burst--;
}