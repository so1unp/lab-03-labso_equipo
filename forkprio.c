#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>      // times()

#define SEGUNDOS 30

int busywork(void)
{
    struct tms buf;
    for (// site_t i=0
        ;// 0 < n 
        ;// i++
    ) {
        times(&buf);
    }
}

// crear un n procesos hijos, con prioridad progresivamente mas baja
int main(int argc, char *argv[])
{
    // arg 1: n hijos a crear
    // arg 2: seg. de ejecucion, 0 indefinido
    // arg 3: si reducen las prioridades (1?)

    // usar
    // fork() crear un proceso
    // setpriority() / nice() | getpriority()
    // sleep()
    // kill()
    // sigaction(SIGTERM, handler, null)    
    while(SEGUNDOS){

    }
    //Luego de un cierto número de segundos se debe envíar una señal de terminación (SIGTERM) a todos los procesos hijos.
    // solucion alarm(cant_seg)
    exit(EXIT_SUCCESS);
}
