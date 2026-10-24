#include "scheduler/scheduler.h"
#include "algorithms/fcfs.h"
#include "algorithms/round_robin.h"
#include "scheduler/dispatcher.h"
#include <stdio.h>
#include <stdlib.h>

static TaskControlBlock* current_task = NULL;
static Queue ready_queues[3];
static Queue* temporal_queue = NULL;

bool scheduler_init(Queue* input_queue) {
    if (input_queue == NULL) {
        return false;
    }

    temporal_queue = input_queue;
    queue_init(&ready_queues[HIGH]);
    queue_init(&ready_queues[MEDIUM]);
    queue_init(&ready_queues[LOW]);
    current_task = NULL;
    return true;
}

void scheduler_destroy(void) {
    int priority;

    free(current_task);
    current_task = NULL;

    for (priority = HIGH; priority <= LOW; priority++) {
        while (!queue_empty(&ready_queues[priority])) {
            free(queue_dequeue(&ready_queues[priority]));
        }
    }

    temporal_queue = NULL;
}

void scheduler_set_current_task(TaskControlBlock* task) {
    current_task = task;
    if (current_task != NULL) {
        current_task->current_status = RUNNING;
    }
}

void scheduler_on_tick(int tick) {
    while (!queue_empty(temporal_queue)) {
        Node* first = queue_front(temporal_queue);
        TaskControlBlock* task = first->data;

        if (task->arrival_time != tick) {
            break;
        }

        task = queue_dequeue(temporal_queue);
        task->current_status = READY;
        if (!queue_enqueue(&ready_queues[task->current_priority], task)) {
            fprintf(stderr,
                    "[ERROR] No se pudo admitir el centinela %d en Q%d.\n",
                    task->id,
                    task->current_priority);
            free(task);
            continue;
        }
        printf("[T-%04d] ARRIVAL: Sentinel ID: %d -> Q%d\n",
               tick,
               task->id,
               task->current_priority);
    }
}

void scheduler_execute_tick(int tick) {
    if (current_task == NULL) {
        int priority;
        for (priority = HIGH; priority <= LOW; priority++) {
            if (!queue_empty(&ready_queues[priority])) {
                scheduler_set_current_task(queue_dequeue(&ready_queues[priority]));
                break;
            }
        }
    }

    if (current_task == NULL) {
        return;
    }

    fcfs_execute_tick(current_task);
    current_task->used_quantum++;

    if (current_task->remaining_burst == 0) {
        current_task->current_status = FINISHED;
        printf("[T-%04d] COMPLETED: Sentinel ID: %d -> Turnaround Time: %d ticks\n",
               tick,
               current_task->id,
               tick - current_task->arrival_time + 1);
        current_task = NULL;
        return;
    }

    if (round_robin_quantum_expired(current_task)) {
        printf("[T-%04d] QUANTUM_EXPIRED: Sentinel %d yields CPU via RR rotation\n",
               tick,
               current_task->id);
        dispatcher_release(current_task);
        current_task = NULL;
    }
}

bool scheduler_has_finished(void) {
    return current_task == NULL;
}