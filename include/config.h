#ifndef CONFIG_H
#define CONFIG_H
#include <stdbool.h>

// Bandera global para habilitar o deshabilitar la traza de depuración detallada.
extern bool VERBOSE_MODE;

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