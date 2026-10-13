#include "scheduler/scheduler.h"

Queue Q0;
Queue Q1;
Queue Q2;

void scheduler_init(void) {
    queue_init(&Q0);
    queue_init(&Q1);
    queue_init(&Q2);
}