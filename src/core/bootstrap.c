#include <stdio.h>
#include <stdlib.h>
#include "../../include/config.h"
#include "../../include/core/bootstrap.h"

int init (int argc, char *argv[]) {
    char *file_path = process_input(argc, argv);
    if (file_path == NULL) return EXIT_FAILURE;
    
    if (VERBOSE_MODE) {
        printf("[DEBUG] Inicializando sistema. Intentando abrir: %s\n", file_path);
    }

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "[ERROR] No se pudo abrir el archivo físico en: %s\n", file_path);
        return EXIT_FAILURE;
    }

    if (VERBOSE_MODE) {
        printf("[DEBUG] Archivo abierto exitosamente. Procesando datos...\n");
    }

    // captura de lineas del archivo de entrada.
    char line[64];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;

        // preparar colas y asignar centinelas. 
    }

    fclose(file);
    return EXIT_SUCCESS;
}
