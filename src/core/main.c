#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "core/bootstrap.h"
#include "scheduler/scheduler.h"

int main(int argc, char *argv[]) {
    Queue* temporal_queue = init(argc, argv);
    if (temporal_queue == NULL) return EXIT_FAILURE;

    if (!scheduler_init(temporal_queue)) {
        queue_destroy(temporal_queue);
        free(temporal_queue);
        return EXIT_FAILURE;
    }

    scheduler_destroy();
    queue_destroy(temporal_queue);
    free(temporal_queue);
    return EXIT_SUCCESS;
}