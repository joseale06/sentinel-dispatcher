#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "config.h"

bool VERBOSE_MODE = false;

char* process_input(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "[ERROR] Faltan argumentos.\n");
        return NULL;
    }

    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "-verbose") == 0) {
        VERBOSE_MODE = true;
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
