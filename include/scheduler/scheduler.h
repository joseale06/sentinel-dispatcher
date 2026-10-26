#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include "control_structures/queue.h"
#include "control_structures/task_control_block.h"

bool scheduler_init(Queue* temporal_queue);
void scheduler_destroy(void);

/**
 * @brief Ejecuta el ciclo de admisión de centinelas para el instante de tiempo actual.
 * 
 * @details Inspecciona el frente de la estructura temporal (planificador a largo plazo).
 *          Todos los centinelas cuyo 'arrival_time' coincida con el valor de 'tick' son
 *          desencolados de la estructura de entrada, su estado pasa a READY y son admitidos
 *          en sus respectivas colas multinivel (Q0, Q1, Q2) según su prioridad establecida.
 * 
 * @param[in] tick Instante del reloj global a evaluar para las admisiones.
 */
void scheduler_on_tick(int tick);

// FUNCIONES DE EVALUACIÓN DE ESTADO.
TaskControlBlock** scheduler_get_active_task_ptr(void);
TaskControlBlock* scheduler_current_task(void);
Queue* scheduler_get_queue(Priority p);

/**
 * @brief Selecciona la cola de listos no vacía con mayor nivel de prioridad del sistema.
 * 
 * @details Aplica la política de planificación a corto plazo con colas multinivel estrictas:
 *          evalúa en orden decreciente Q0 (HIGH), Q1 (MEDIUM) y Q2 (LOW). Devuelve el puntero
 *          a la primera cola que contenga centinelas listos sin alterar estructuras ni
 *          extraer ningún nodo (responsabilidad delegada al despachador).
 * 
 * @return Puntero a la Queue con mayor prioridad que tenga trabajo disponible, o NULL si
 *         todas las colas de listos están vacías.
 */
Queue* scheduler_select_highest_priority_queue(void);

/**
 * @brief Actualiza la métrica de tiempo de espera de los centinelas encolados.
 * 
 * @details Recorre todos los nodos enlazados en las colas Q0, Q1 y Q2 que no están
 *          ejecutando en el procesador e incrementa en 1 su contador 'wait_time'.
 *          Garantiza el cálculo acumulativo exacto para las métricas de rendimiento global.
 */
void scheduler_update_waiting_metrics(void);

/**
 * @brief Evalúa la condición de parada absoluta de la simulación.
 * 
 * @details Comprueba simultáneamente que la CPU se encuentre desocupada ('current_task == NULL'),
 *          que la estructura temporal de admisiones futuras esté vacía, y que ninguna de las
 *          tres colas de listos (HIGH, MEDIUM, LOW) contenga centinelas pendientes.
 * 
 * @return true si todo el trabajo del sistema concluyó; false en caso contrario.
 */
bool scheduler_has_finished(void);

void scheduler_record_completion(const TaskControlBlock* task, int tick);
void scheduler_print_metrics(void);

#endif