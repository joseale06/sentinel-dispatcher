#include <stdio.h>
#include "queue_policies/rr_policy.h"
#include "config.h"

int round_robin_quantum(Priority priority) {
    switch (priority) {
        case HIGH:
            return QUANTUM_Q0;
        case MEDIUM:
            return QUANTUM_Q1;
        case LOW:
        default:
            return 0;
    }
}

bool round_robin_quantum_expired(const TaskControlBlock* task) {
    int quantum = round_robin_quantum(task->current_priority);
    return quantum > 0 && task->used_quantum >= quantum;
}