#include <stdio.h>
#include "execution_sync/work_thread.h"
#include "execution_sync/main_thread.h"
#include "scheduler/scheduler.h"
#include "scheduler/dispatcher.h"
#include "queue_policies/fcfs_policy.h"
#include "queue_policies/rr_policy.h"
#include "config.h"

static bool should_preempt(TaskControlBlock* active_task) {
    if (active_task == NULL) return false;

    // si la tarea activa es de Q1 (MEDIUM), cualquier tarea en Q0 (HIGH) la desaloja.
    if (active_task->current_priority == MEDIUM) {
        Queue* q0 = scheduler_get_queue(HIGH);
        return !queue_empty(q0);
    }

    // si la tarea activa es de Q2 (LOW), tareas en Q0 o Q1 la desaloja.
    if (active_task->current_priority == LOW) {
        Queue* q0 = scheduler_get_queue(HIGH);
        Queue* q1 = scheduler_get_queue(MEDIUM);
        return (!queue_empty(q0) || !queue_empty(q1));
    }

    // tareas en Q0 (HIGH) nunca son expropiadas por prioridad.
    return false;
}

void* work_thread_routine(void* arg) {
    (void)arg;

    while (!simulation_finished) {
        pthread_mutex_lock(&tick_mutex);

        // esperar a que el 'Hilo Maestro' complete la admisión de este tic.
        while (!master_done && !simulation_finished) {
            pthread_cond_wait(&cond_worker, &tick_mutex);
        }

        if (simulation_finished) {
            pthread_mutex_unlock(&tick_mutex);
            break;
        }

        TaskControlBlock** active_task_ptr = scheduler_get_active_task_ptr();
        TaskControlBlock* active_task = *active_task_ptr;

        // evaluación del centinela que ejecutó en el tic anterior
        if (active_task != NULL) {
            if (active_task->remaining_burst == 0) {
                // Tarea terminada en el tic anterior -> Imprime COMPLETED y libera memoria
                dispatcher_finish_task(active_task_ptr, GLOBAL_TIC);
            } else if (round_robin_quantum_expired(active_task)) {
                // Fase 2: Expiración de quantum -> Rota al final de su cola
                Queue* target_q = scheduler_get_queue(active_task->current_priority);
                dispatcher_quantum_expired(active_task_ptr, target_q, GLOBAL_TIC);
            } else if (should_preempt(active_task)) {
                // Fase 3: Expropiación -> Desaloja a la cabeza de su cola
                Queue* target_q = scheduler_get_queue(active_task->current_priority);
                dispatcher_preempt(active_task_ptr, target_q, GLOBAL_TIC);
            }
        }

        // despacho si el procesador está desocupado
        if (*active_task_ptr == NULL) {
            Queue* highest_q = scheduler_select_highest_priority_queue();
            if (highest_q != NULL) {
                dispatcher_context_switch(active_task_ptr, highest_q);
            }
        }

        // ejecución física de 1 unidad de ráfaga para el tic actual
        active_task = *active_task_ptr;
        if (active_task != NULL) {
            fcfs_execute_tick(active_task);
            active_task->used_quantum++;
        }

        scheduler_update_waiting_metrics();

        master_done = false;
        worker_done = true;
        pthread_cond_signal(&cond_master);

        pthread_mutex_unlock(&tick_mutex);
    }

    return NULL;
}