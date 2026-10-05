#ifndef CONFIG_H
#define CONFIG_H
#include <stdbool.h>

#define QUANTUM_Q0 = 2; // quantum de tiempo para cola 0 (alta prioridad).
#define QUANTUM_Q1 = 4; // quantum de tiempo para cola 1 (prioridad media).

extern bool VERBOSE_MODE; // bandera global para la traza de depuración detallada.

/**
 * @brief Reloj discreto del simulador (Tg).
 * @details Avanza en unidades enteras (tics de reloj). Gestionado e incrementado
 *          por el Hilo Maestro durante la simulación.
 */
extern int GLOBAL_TIC; 

/** 
* @brief Parsea y valida los argumentos de la linea de comandos para configurar el entorno.
* @details Valida la presencia de las banderas obligatorias, activa el modo de depuración
*          si se indica '-v' o '-verbose', y aísla la ruta del archivo de entrada.
*
* @param argc Cantidad de argumentos indicados por consola.
* @param argv Arreglo de strings con los argumentos reales.
*
* @return Puntero a la cadena con la ruta del archivo de entrada si la sintaxis es válida.
* @retval NULL si la entrada no es válida.
* 
* @note No reserva memoria dinámica; el puntero retornado apunta directamente a argv.
*/
char* process_input(int argc, char *argv[]);

#endif