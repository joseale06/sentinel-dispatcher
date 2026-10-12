#include "execution_sync/main_thread.h"
#include "control_structures/task_control_block.h"
#include <stdio.h>

int global_tick = 0;
bool simulation_finished = false;

pthread_mutex_t tick_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_master = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_worker = PTHREAD_COND_INITIALIZER;

bool master_done = false;
bool worker_done = true; 

void* main_thread_routine(void* arg) {
    
    while (!simulation_finished) {
        pthread_mutex_lock(&tick_mutex);

        while (!worker_done && !simulation_finished) {
            pthread_cond_wait(&cond_master, &tick_mutex);
        }

        if (simulation_finished) {
            pthread_mutex_unlock(&tick_mutex);
            break;
        }
        
        // 1. Verificar qué centinelas tienen arrivaltime == global_tick
        // 2. Encolarlos en las estructuras (llamando a dispatcher.h / scheduler.h)
        // 3. Imprimir el log de ARRIVAL si corresponde
        
        // La lógica de impresión Verbose se maneja aquí al final del tic

        global_tick++;

        worker_done = false;
        master_done = true;
        pthread_cond_signal(&cond_worker);

        pthread_mutex_unlock(&tick_mutex);
    }

    return NULL;
}