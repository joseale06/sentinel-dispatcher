#include <stdio.h>
#include <string.h>
#include "../include/config.h"

int VERBOSE_MODE = 0;

/*
@brief Parsea y valida los argumentos de la linea de comandos para configurar el entorno.
@details Valida la presencia de las banderas obligatorias, activa el modo de depuración
         si se indica '-v' o '-verbose', y aísla la ruta del archivo de entrada.

@param : argc Cantidad de argumentos indicados por consola.
@param : argv Arreglo de strings con los argumentos reales.

@return : Puntero a la cadena con la ruta del archivo de entrada si la sintaxis es válida.
@retval : NULL si la entrada no es válida.

@note No reserva memoria dinámica; el puntero retornado apunta directamente a argv.
*/
char* process_input(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "[ERROR] Faltan argumentos.\n");
        return NULL;
    }

    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "-verbose") == 0) {
        VERBOSE_MODE = 1;
        if (argc < 4 || strcmp(argv[2], "--input") != 0) {
            fprintf(stderr, "[ERROR] Sintaxis inválida tras flag verbose.\n");
            return NULL;
        }
        return argv[3];
    } 
    else if (strcmp(argv[1], "--input") == 0) {
        return argv[2];
    } 
    
    fprintf(stderr, "[ERROR] Argumento inicial no reconocido.\n");
    return NULL;
}
