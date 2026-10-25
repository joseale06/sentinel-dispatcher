#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "config.h"
#include "core/bootstrap.h"
#include "control_structures/queue.h"
#include "control_structures/task_control_block.h"

Queue* init (int argc, char *argv[]) {
    char *file_path = process_input(argc, argv);
    if (file_path == NULL) return NULL;
    
    if (VERBOSE_MODE) {
        printf("[DEBUG] Initializing system. Attempting to open: %s\n", file_path);
    }

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "[ERROR] The physical file could not be opened at: %s\n", file_path);
        return NULL;
    }

    if (VERBOSE_MODE) {
        printf("[DEBUG] File successfully opened. Processing data...\n");
    }

    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL) {
        fprintf(stderr, "[ERROR] Memory allocation error.\n");
        fclose(file);
        return NULL;
    }
    queue_init(q);

    // captura de lineas del archivo de entrada.
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\0') continue; // ignorar comentarios o lineas vacías.

        int arrival_time, id, priority, total_burst;
        int parsed_items = sscanf(line, "%d %d %d %d", &arrival_time, &id, &priority, &total_burst);
        
        if (parsed_items != 4) {
            fprintf(stderr, "[WARN] Line with invalid format omitted: %s", line);
            continue;
        }

        if (priority < 0 || priority > 2 || total_burst <= 0 || arrival_time < 0) {
            fprintf(stderr, "[WARN] Sentinel with out-of-range values omitted: ID%d\n", id);
            continue;
        }

        TaskControlBlock *new_tcb = TCB_create(id, arrival_time, (Priority)priority, total_burst);
        if (new_tcb == NULL) {
            fprintf(stderr, "[ERROR] Memory allocation error.\n");
            queue_destroy(q);
            free(q);
            fclose(file);
            return NULL;
        }

        if (!queue_enqueue(q, new_tcb)) {
            fprintf(stderr, "[ERROR] Sentinel %d could not be registered in the temporal structure.\n", id);
            free(new_tcb);
            queue_destroy(q);
            free(q);
            fclose(file);
            return NULL;
        }

        if (VERBOSE_MODE) {
            printf("[DEBUG] TCB created and inserted -> ID: %d | Arrival: %d | Priority: %d | Total Burst: %d\n",
                   new_tcb->id,
                   new_tcb->arrival_time,
                   new_tcb->current_priority,
                   new_tcb->total_burst);
        }
    }
    fclose(file);
    queue_sort(q); // ordenamiento de la estructura temporal para registrar a los centinelas.
    if (VERBOSE_MODE) queue_print(q);
    
    return q;
}
