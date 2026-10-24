#include "scheduler/dispatcher.h"

void dispatcher_release(TaskControlBlock* task) {
    task->current_status = READY;
    task->used_quantum = 0;
}