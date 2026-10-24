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
 * @return Cola temporal ordenada por tiempo de llegada, o NULL si ocurre un error.
 */
Queue* init (int argc, char *argv[]);

#endif