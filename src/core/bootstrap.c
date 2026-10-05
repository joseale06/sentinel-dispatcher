#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "config.h"
#include "core/bootstrap.h"
#include "control_structures/queue.h"
#include "control_structures/task_control_block.h"

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

    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL) {
        fprintf(stderr, "[ERROR] Fallo de asignacion de memoria.\n");
        fclose(file);
        return EXIT_FAILURE;
    }
    queue_init(q);

    // captura de lineas del archivo de entrada.
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\0') continue; // ignorar comentarios o lineas vacías.

        int arrival_time, id, priority, total_burst;
        int parsed_items = sscanf(line, "%d %d %d %d", &arrival_time, &id, &priority, &total_burst);
        
        if (parsed_items != 4) {
            fprintf(stderr, "[WARN] Linea con formato invalido omitida: %s", line);
            continue;
        }

        if (priority < 0 || priority > 2 || total_burst <= 0 || arrival_time < 0) {
            fprintf(stderr, "[WARN] Centinela con valores fuera de rango omitido: ID %d\n", id);
            continue;
        }

        TaskControlBlock *new_tcb = TCB_create(id, arrival_time, (Priority)priority, total_burst);
        if (new_tcb == NULL) {
            fprintf(stderr, "[ERROR] Error de memoria para instanciar centinela %d.\n", id);
            queue_destroy(q);
            free(q);
            fclose(file);
            return EXIT_FAILURE;
        }

        if (!queue_enqueue(q, new_tcb)) {
            fprintf(stderr, "[ERROR] Fallo al registrar centinela %d en estructura temporal.\n", id);
            free(new_tcb);
            queue_destroy(q);
            free(q);
            fclose(file);
            return EXIT_FAILURE;
        }

        if (VERBOSE_MODE) {
            printf("[DEBUG] TCB creado e insertado -> ID: %d | Arribo: %d | Prioridad: %d | Rafaga Total: %d\n",
                   new_tcb->id,
                   new_tcb->arrival_time,
                   new_tcb->current_priority,
                   new_tcb->total_burst);
        }
    }
    fclose(file);
    queue_sort(q); // ordenamiento de la estructura temporal para registrar a los centinelas.
    if (VERBOSE_MODE) queue_print(q);
    
    return EXIT_SUCCESS;
}
