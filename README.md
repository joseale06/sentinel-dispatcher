sentinel-dispatcher

make run ARGS="-v --input ataque_sion.txt"

En cada tic, `main_thread.c` controla el reloj y la admisión de llegadas,
mientras `work_thread.c` orquesta el ciclo de CPU: solicita el despacho al
scheduler, ejecuta una unidad de ráfaga y actualiza las métricas del proceso.