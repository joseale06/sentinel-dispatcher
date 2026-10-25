#ifndef WORK_THREAD_H
#define WORK_THREAD_H

#include <stdbool.h>
#include "control_structures/task_control_block.h"

void* work_thread_routine(void* arg);

#endif