#include "algorithms/fcfs.h"

void fcfs_execute_tick(TaskControlBlock* task) {
    task->remaining_burst--;
}