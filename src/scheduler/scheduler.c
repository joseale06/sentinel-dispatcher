#include <stdio.h>
#include <stdlib.h>
#include "scheduler/scheduler.h"

static TaskControlBlock* current_task = NULL;
static Queue ready_queues[3];
static Queue* temporal_queue = NULL;

bool scheduler_init(Queue* input_queue) {
    if (input_queue == NULL) return false;
    
    temporal_queue = input_queue;
    queue_init(&ready_queues[HIGH]);
    queue_init(&ready_queues[MEDIUM]);
    queue_init(&ready_queues[LOW]);
    current_task = NULL;
    return true;
}

void scheduler_destroy(void) {
    int priority;
    if (current_task != NULL) {
        free(current_task);
        current_task = NULL;
    }

    for (priority = HIGH; priority <= LOW; priority++) {
        while (!queue_empty(&ready_queues[priority])) {
            free(queue_dequeue(&ready_queues[priority]));
        }
    }
    temporal_queue = NULL;
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
                    "[ERROR] Sentinel %d could not be accepted in Q%d.\n",
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

TaskControlBlock** scheduler_get_active_task_ptr(void) {
    return &current_task;
}

TaskControlBlock* scheduler_current_task(void) {
    return current_task;
}

Queue* scheduler_get_queue(Priority p) {
    return &ready_queues[p];
}

Queue* scheduler_select_highest_priority_queue(void) {
    int priority;
    for (priority = HIGH; priority <= LOW; priority++) {
        if (!queue_empty(&ready_queues[priority])) {
            return &ready_queues[priority];
        }
    }
    return NULL;
}

void scheduler_update_waiting_metrics(void) {
    int priority;
    Node* node;
    for (priority = HIGH; priority <= LOW; priority++) {
        node = queue_front(&ready_queues[priority]);
        while (node != NULL) {
            node->data->wait_time++;
            node = node->next;
        }
    }
}

bool scheduler_has_finished(void) {
    int priority;
    if (current_task != NULL) return false;
    if (!queue_empty(temporal_queue)) return false;
    
    for (priority = HIGH; priority <= LOW; priority++) {
        if (!queue_empty(&ready_queues[priority])) return false;
    }
    
    return true;
}