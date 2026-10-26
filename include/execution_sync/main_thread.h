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

/**
 * @brief Rutina principal del hilo maestro.
 * 
 * @details Controla el avance del reloj global (GLOBAL_TIC) y la sincronización por fases:
 *          1. Espera a que el hilo trabajador culmine el ciclo de CPU del tic anterior.
 *          2. Ejecuta la Fase 1 (admisión de centinelas que llegan en Tg).
 *          3. Evalúa la condición global de terminación ('scheduler_has_finished()').
 *          4. Señaliza al hilo trabajador mediante banderas para habilitar el ciclo de cómputo.
 *          5. Tras la finalización del cómputo de la CPU en el mismo tic, incrementa GLOBAL_TIC.
 * 
 * @param[in] arg Puntero a argumentos de inicialización de pthread (no utilizado, pasar NULL).
 * @return Siempre retorna NULL al concluir la simulación.
 */
void* main_thread_routine(void* arg);

#endif