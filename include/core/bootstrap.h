#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

#include "control_structures/queue.h"

/**
 * @brief Inicializa el subsistema de carga y devuelve la cola temporal.
 * @details Gestiona el parseo de argumentos CLI, valida la existencia del archivo físico,
 *          reserva la memoria para una estructura temporal que almacena los centinelas y 
 *          valida los datos recibidos antes de comenzar la simulación.
 *
 * @param argc Cantidad de argumentos recibidos en main.
 * @param argv Vector de argumentos recibido en main.
 *
 * @return Puntero a la cola (Queue*) que contiene la 'estructura temporal' ordenada.
 * @retval NULL Si ocurrió un error de sintaxis, archivo o reserva de memoria.
 */
Queue* init (int argc, char *argv[]);

#endif