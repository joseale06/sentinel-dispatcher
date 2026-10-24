#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

/**
 * @brief Inicializa el subsistema de carga y orquesta la lectura de escenarios.
 * @details Gestiona el parseo de argumentos CLI, valida la existencia del archivo físico,
 *          reserva la memoria para una estructura temporal que almacena los centinelas y 
 *          valida los datos recibidos antes de comenzar la simulación.
 *
 * @param argc Cantidad de argumentos recibidos en main.
 * @param argv Vector de argumentos recibido en main.
 *
 * @return Código de estado para el sistema operativo.
 * @retval EXIT_SUCCESS Si el entorno y los datos se cargaron satisfactoriamente.
 * @retval EXIT_FAILURE Si ocurrió un error de sintaxis, archivo o reserva de memoria.
 */
int init (int argc, char *argv[]);

#endif