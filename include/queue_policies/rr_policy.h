#ifndef RR_POLICY_H
#define RR_POLICY_H

#include <stdbool.h>
#include "control_structures/task_control_block.h"

int round_robin_quantum(Priority priority);
bool round_robin_quantum_expired(const TaskControlBlock* task);

#endif