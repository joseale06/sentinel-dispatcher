#include "execution_sync/work_thread.h"
#include "execution_sync/main_thread.h"
#include "control_structures/task_control_block.h"
#include <stdio.h>
#include <pthread.h>

void* work_thread_routine(void* arg) {
    TaskControlBlock* current_cpu_task = NULL; 

    while (!simulation_finished) {
        pthread_mutex_lock(&tick_mutex);

        while (!master_done && !simulation_finished) {
            pthread_cond_wait(&cond_worker, &tick_mutex);
        }

        if (simulation_finished) {
            pthread_mutex_unlock(&tick_mutex);
            break;
        }

        // FASE DE TRABAJADOR CPU_0
        if (current_cpu_task != NULL) {

            current_cpu_task->remaining_burst--;
            current_cpu_task->used_quantum++;

            // Evaluar terminación
            if (current_cpu_task->remaining_burst == 0) {
                current_cpu_task->current_status = FINISHED; // Usando tu enum[cite: 7]
                int turnaround = global_tick - current_cpu_task->arrival_time + 1;
                printf("[T-%04d] COMPLETED: Sentinel ID: %d -> Turnaround Time: %d ticks\n", 
                       global_tick, current_cpu_task->id, turnaround);
                
                current_cpu_task = NULL; 
            }

            else if ((current_cpu_task->current_priority == HIGH && current_cpu_task->used_quantum == 2) ||
                     (current_cpu_task->current_priority == MEDIUM && current_cpu_task->used_quantum == 4)) {
                
                printf("[T-%04d] QUANTUM_EXPIRED: Sentinel %d yields CPU via RR rotation\n", 
                       global_tick, current_cpu_task->id);
                
                current_cpu_task->current_status = READY;
                current_cpu_task = NULL; 
            }
        }

        master_done = false;
        worker_done = true;
        pthread_cond_signal(&cond_master);

        pthread_mutex_unlock(&tick_mutex);
    }

    return NULL;
}