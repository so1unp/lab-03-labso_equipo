#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h> // times()
#include <signal.h>

#define SEGUNDOS 3

int busywork(void)
{
    struct tms buf;
    for ( // site_t i=0
        ; // 0 < n
        ; // i++
    )
    {
        times(&buf);
    }
}
void catch_alarm(int sig_num)
{
    printf("\nDemoraste demasiado!. Me estoy yendo...\n");
    exit(0);
}

// crear un n procesos hijos, con prioridad progresivamente mas baja
int main(int argc, char *argv[])
{
    // arg 1: n hijos a crear
    // arg 2: seg. de ejecucion, 0 indefinido
    // arg 3: si reducen las prioridades (1?)
    if (argc - 1 != 3)
    {
        exit(EXIT_FAILURE);
    }

    // usar
    // fork() crear un proceso
    // setpriority() / nice() | getpriority()
    // sleep()
    // kill()

    struct sigaction sa;
    sa.sa_handler = catch_alarm;
    sigaction(SIGALRM, &sa, NULL);
    alarm(atoi(argv[2]));

    // printf("Child %d (nice %2d):\t%3li\n");
    pause();
    // Luego de un cierto número de segundos se debe envíar una señal de terminación (SIGTERM) a todos los procesos hijos.
    //  solucion alarm(cant_seg)
    exit(EXIT_SUCCESS);
}
