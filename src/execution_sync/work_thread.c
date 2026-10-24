#include "execution_sync/work_thread.h"
#include "execution_sync/main_thread.h"
#include "scheduler/scheduler.h"
#include "algorithms/fcfs.h"

void* work_thread_routine(void* arg) {
    (void)arg;

    while (!simulation_finished) {
        pthread_mutex_lock(&tick_mutex);

        while (!master_done && !simulation_finished) {
            pthread_cond_wait(&cond_worker, &tick_mutex);
        }

        if (simulation_finished) {
            pthread_mutex_unlock(&tick_mutex);
            break;
        }

        TaskControlBlock* active_task = scheduler_dispatch();
        if (active_task != NULL) {
            fcfs_execute_tick(active_task);
            active_task->used_quantum++;
            active_task->return_time =
                global_tick - active_task->arrival_time + 1;
        }
        scheduler_update_waiting_metrics();
        scheduler_finish_cpu_tick(global_tick);
        master_done = false;
        worker_done = true;
        pthread_cond_signal(&cond_master);

        pthread_mutex_unlock(&tick_mutex);
    }

    return NULL;
}