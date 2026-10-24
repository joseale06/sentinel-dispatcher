#include "execution_sync/work_thread.h"
#include "execution_sync/main_thread.h"
#include "scheduler/scheduler.h"

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

        scheduler_execute_tick(global_tick);
        master_done = false;
        worker_done = true;
        pthread_cond_signal(&cond_master);

        pthread_mutex_unlock(&tick_mutex);
    }

    return NULL;
}