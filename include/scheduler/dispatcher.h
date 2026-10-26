#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "control_structures/task_control_block.h"
#include "control_structures/queue.h"

/**
 * @brief Realiza el cambio de contexto asignando un nuevo centinela al procesador.
 * 
 * @details Extrae físicamente el nodo del frente de la cola de listos especificada
 *          y asigna su dirección directamente a la variable de CPU mediante la
 *          desreferenciación del doble puntero. Transiciona el estado del TCB
 *          a RUNNING.
 * 
 * @param[in,out] cpu_task Puntero doble a la tarea activa en CPU. Se actualiza atómicamente
 *                         con la dirección del TCB desencolado.
 * @param[in,out] source_queue Cola de prioridad seleccionada de la cual se extraerá el centinela.
 */
void dispatcher_context_switch(TaskControlBlock** cpu_task, Queue* source_queue);

/**
 * @brief Ejecuta la rotación por expiración de quantum de la política Round Robin.
 * 
 * @details Modifica el estado del centinela activo a READY, reinicia a cero su contador
 *          de quantum consumido ('used_quantum = 0') y lo encola al final de su cola
 *          correspondiente. Finalmente, limpia la referencia del procesador escribiendo
 *          NULL a través del doble puntero para indicar que la CPU quedó vacía.
 * 
 * @param[in,out] cpu_task Puntero doble a la tarea activa en CPU. Queda apuntando a NULL tras la expulsión.
 * @param[in,out] target_queue Cola de origen a cuyo final se reinsertará el centinela expulsado.
 * @param[in] current_tick Estampa de tiempo del reloj global utilizada para emitir la traza de log.
 */
void dispatcher_quantum_expired(TaskControlBlock** cpu_task, Queue* target_queue, int current_tick);

/**
 * @brief Desaloja al proceso en ejecución debido a la llegada de un proceso con mayor prioridad.
 * 
 * @details Transiciona el centinela a estado READY y lo reinserta al FRENTE (head) de su cola
 *          mediante 'queue_push_front()' para preservar su turno prioritario una vez se desocupe
 *          el procesador. Mantiene intactas las unidades de ráfaga restante y quantum
 *          consumido (a menos que se haya reiniciado previamente por caso borde). Limpia la CPU
 *          dejando el puntero en NULL.
 * 
 * @param[in,out] cpu_task Puntero doble a la tarea activa en CPU. Queda apuntando a NULL tras el desalojo.
 * @param[in,out] target_queue Cola de nivel correspondiente donde se inserta al frente la tarea.
 * @param[in] current_tick Estampa de tiempo del reloj global para la traza de auditoría.
 */
void dispatcher_preempt(TaskControlBlock** cpu_task, Queue* target_queue, int current_tick);

/**
 * @brief Concluye el ciclo de vida de un centinela que agotó su ráfaga de CPU.
 * 
 * @details Marca el estado de la tarea como FINISHED, calcula su tiempo total de retorno
 *          (Turnaround Time: 'current_tick - arrival_time'), emite la traza formateada por
 *          consola, libera la estructura de memoria dinámica del TCB y establece a NULL la 
 *          referencia en la CPU para dejarla disponible para el siguiente despacho.
 * 
 * @param[in,out] cpu_task Puntero doble a la tarea activa en CPU. La memoria apuntada es
 *                         liberada y el puntero se establece en NULL.
 * @param[in] current_tick Tic global en el que concluye la ejecución de la tarea.
 */
void dispatcher_finish_task(TaskControlBlock** cpu_task, int current_tick);

#endif
