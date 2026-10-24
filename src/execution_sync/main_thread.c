#include "execution_sync/main_thread.h"
#include "scheduler/scheduler.h"

int global_tick = 0;
bool simulation_finished = false;

pthread_mutex_t tick_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_master = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_worker = PTHREAD_COND_INITIALIZER;

bool master_done = false;
bool worker_done = true; 

void* main_thread_routine(void* arg) {
    (void)arg;
    
    while (!simulation_finished) {
        pthread_mutex_lock(&tick_mutex);

        while (!worker_done && !simulation_finished) {
            pthread_cond_wait(&cond_master, &tick_mutex);
        }

        if (simulation_finished) {
            pthread_mutex_unlock(&tick_mutex);
            break;
        }
        
        scheduler_on_tick(global_tick);
        global_tick++;

        worker_done = false;
        master_done = true;
        pthread_cond_signal(&cond_worker);

        pthread_mutex_unlock(&tick_mutex);
    }

    return NULL;
}