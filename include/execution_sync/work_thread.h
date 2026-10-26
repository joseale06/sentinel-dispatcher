#ifndef WORK_THREAD_H
#define WORK_THREAD_H

#include <stdbool.h>
#include "control_structures/task_control_block.h"

/**
 * @brief Rutina del hilo trabajador.
 * 
 * @details Orquesta la ejecución secuencial de las fases de hardware dentro del ciclo de reloj:
 *          - Permanece bloqueado hasta que el hilo maestro concluye la admisión.
 *          - Evalúa el proceso activo del tic anterior: terminación ('remaining_burst == 0'),
 *            expiración de quantum (Fase 2, distinguiendo rotación si hay contendientes o
 *            renovación silenciosa si la cola está vacía), o expropiación por llegada de mayor
 *            prioridad (Fase 3, resolviendo el caso borde de solapamiento con quantum expirado).
 *          - Si el procesador queda libre, invoca la Fase 4 de despacho.
 *          - Ejecuta 1 tic de ráfaga física sobre el centinela activo y actualiza las métricas
 *            de espera de las colas de listos.
 *          - Notifica la conclusión del ciclo de CPU al hilo maestro a través de 'cond_master'.
 * 
 * @param[in] arg Puntero a argumentos de inicialización de pthread (no utilizado, pasar NULL).
 * @return Siempre retorna NULL al culminar la simulación.
 */
void* work_thread_routine(void* arg);

#endif