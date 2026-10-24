#ifndef TASK_CONTROL_BLOCK_H
#define TASK_CONTROL_BLOCK_H

typedef enum {
    NEW,
    READY,
    RUNNING,
    INTERRUPTED,
    FINISHED
} ExecutionStatus;

typedef enum {
    HIGH,
    MEDIUM,
    LOW
} Priority;

typedef struct TaskControlBlock {
    int id;
    ExecutionStatus current_status;
    Priority current_priority;
    int arrival_time; // tiempo de llegada al sistema.
    int total_burst; // ráfaga de CPU total de procesamiento requerida.
    int remaining_burst; // ráfaga de CPU restante de procesamiento.
    int used_quantum; // quantum consumido acumulado en la ráfaga actual.
    int wait_time;
    int return_time;
} TaskControlBlock;

TaskControlBlock* TCB_create(int id, int arrival_time, Priority priority, int total_burst);

#endif