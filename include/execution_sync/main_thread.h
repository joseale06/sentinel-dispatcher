#ifndef MAIN_THREAD_H
#define MAIN_THREAD_H

#include <pthread.h>
#include <stdbool.h>

extern bool simulation_finished;

extern pthread_mutex_t tick_mutex;
extern pthread_cond_t cond_master;
extern pthread_cond_t cond_worker;

extern bool master_done;
extern bool worker_done;

void* main_thread_routine(void* arg);

#endif