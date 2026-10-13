#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "core/bootstrap.h"
#include "control_structures/queue.h"
#include "scheduler/scheduler.h"
#include "execution_sync/main_thread.h"
#include "execution_sync/work_thread.h"

int main(int argc, char *argv[]) {
    Queue* q = init(argc, argv);
    if (q == NULL) {
        fprintf(stderr, "[FATAL] Fallo critico en la inicializacion. Abortando.\n");
        return EXIT_FAILURE;
    }

    scheduler_init();

    // definición de los identificadores de los hilos POSIX.
    pthread_t main_thread, work_thread;

/* Se deben establecer las rutinas respectivas con el nombre exacto.
    if (pthread_create(&main_thread, NULL, main_thread_routine, (void*)q) != 0) {
        fprintf(stderr, "[FATAL] No se pudo crear el Hilo Maestro.\n");
        return EXIT_FAILURE;
    }

    if (pthread_create(&work_thread, NULL, work_thread_routine, NULL) != 0) {
        fprintf(stderr, "[FATAL] No se pudo crear el Hilo Trabajador.\n");
        return EXIT_FAILURE;
    }

    pthread_join(main_thread, NULL);
    pthread_join(work_thread, NULL);
*/
    queue_destroy(q);
    free(q);

    return EXIT_SUCCESS;
}