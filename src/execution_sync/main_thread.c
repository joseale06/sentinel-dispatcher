#include "execution_sync/main_thread.h"
#include "scheduler/scheduler.h"
#include "config.h"

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

        // esperar a que 'Hilo Trabajador' termine el ciclo de CPU del tic anterior.
        while (!worker_done && !simulation_finished) {
            pthread_cond_wait(&cond_master, &tick_mutex);
        }

        if (simulation_finished) {
            pthread_mutex_unlock(&tick_mutex);
            break;
        }
        
        // admisión para el tic actual.
        scheduler_on_tick(GLOBAL_TIC);

        // verifica si ya no hay tareas en ninguna estructura ni en CPU.
        if (scheduler_has_finished()) {
            simulation_finished = true;
            master_done = true;
            pthread_cond_signal(&cond_worker);
            pthread_mutex_unlock(&tick_mutex);
            break;
        }

        // habilita a 'Hilo Trabajador' para ejecutar el ciclo de CPU de este tic.
        worker_done = false;
        master_done = true;
        pthread_cond_signal(&cond_worker);

        // espera a que la CPU termine el trabajo de este mismo tic antes de avanzar el reloj.
        while (master_done && !simulation_finished) {
            pthread_cond_wait(&cond_master, &tick_mutex);
        }

        GLOBAL_TIC++;
        pthread_mutex_unlock(&tick_mutex);
    }

    return NULL;
}