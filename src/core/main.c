#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "core/bootstrap.h"
#include "control_structures/queue.h"
#include "scheduler/scheduler.h"
#include "execution_sync/main_thread.h"
#include "execution_sync/work_thread.h"

int main(int argc, char *argv[]) {
    Queue* temporal_queue = init(argc, argv);
    if (temporal_queue == NULL) return EXIT_FAILURE;
    
    if (!scheduler_init(temporal_queue)) {
        queue_destroy(temporal_queue);
        free(temporal_queue);
        return EXIT_FAILURE;
    }

    // definición de los identificadores de los hilos POSIX.
    pthread_t master_thread, worker_thread;

    if (pthread_create(&master_thread, NULL, main_thread_routine, NULL) != 0) {
        fprintf(stderr, "[ERROR] The Master Thread could not be created.\n");
        scheduler_destroy();
        queue_destroy(temporal_queue);
        free(temporal_queue);
        return EXIT_FAILURE;
    }

    if (pthread_create(&worker_thread, NULL, work_thread_routine, NULL) != 0) {
        fprintf(stderr, "[ERROR] The Worker Thread could not be created.\n");
        simulation_finished = true;
        pthread_join(master_thread, NULL);
        scheduler_destroy();
        queue_destroy(temporal_queue);
        free(temporal_queue);
        return EXIT_FAILURE;
    }

    pthread_join(master_thread, NULL);
    pthread_join(worker_thread, NULL);

    scheduler_destroy();
    queue_destroy(temporal_queue);
    free(temporal_queue);
    
    return EXIT_SUCCESS;
}