#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/times.h> // times()
#include <sys/types.h>
#include <sys/wait.h>
int *buffer;

int valid(int argc, char *argv[]);
void termHijos(int *buff, int l);
void terminar(int pid);

// funcion invocada por los procesos hijos
int busywork(void) {
    struct tms buf;
    for (;;) {
        times(&buf);
    }
}

void handler() {
    int prioridad = getpriority(PRIO_PROCESS, 0);
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    long int sum = usage.ru_utime.tv_sec + usage.ru_stime.tv_sec;

    printf("Child %d (nice %2d):\t%3li\n", getpid(), prioridad, sum);
    exit(0);
}

void handler_padre() {
    printf("Signal SIGINT recibida. Terminando el programa...\n");
}

int main(int argc, char *argv[])
{
    struct sigaction sac, sap;
    sac.sa_handler = handler;
    sap.sa_handler = handler_padre;

    if (valid(argc, argv) == 0) {
        exit(EXIT_FAILURE);
    }

    int _hijos = atoi(argv[1]);
    int _segundos = atoi(argv[2]);
    int _prio = atoi(argv[3]);

    buffer = (int *)malloc((size_t)_hijos * sizeof(int)); // arreglo con los pid de los procesos hijos

    for (int i = 0; i < _hijos; i++) {
        pid_t child_pid = fork();
        if (child_pid < 0) {
            perror("error");
            exit(EXIT_FAILURE);
        }
        if (child_pid == 0) {
            sigaction(SIGTERM, &sac, NULL);
            if (_prio) {
                if (nice(i) == -1) {
                    perror("Error al asignar prioridad");
                }
            }
            busywork();
        } else {
            buffer[i] = child_pid;
        }
    }

     // enviar signal desde htop
    if (!_segundos) {
        sigaction(SIGINT, &sap, NULL);
        printf("espera indefinida...\n");
        pause();
    } else {
        sleep((unsigned int)_segundos);
    }

    termHijos(buffer, _hijos);
    free(buffer);

    exit(EXIT_SUCCESS);
}

int valid(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s hijos segundos prioridad\n", argv[0]);
        fprintf(stderr, "\thijos        Numero de hijos a generar.\n");
        fprintf(stderr, "\tsegundos     Tiempo que se debe ejecutar, 0 indefinido.\n");
        fprintf(stderr, "\tprioridad    Si se reducen las prioridades 1/0 .\n");
        return 0;
    }
    int h = atoi(argv[1]);
    int s = atoi(argv[2]);
    int r = atoi(argv[3]);

    if (h < 0) {
        fprintf(stderr, "Error: el numero de hijos debe ser mayor que cero.\n");
        return 0;
    }
    if (s < 0) {
        fprintf(stderr, "Error: los segundos deben ser mayor o igual que cero.\n");
        return 0;
    }
    if (r != 0 && r != 1) {
        fprintf(stderr, "Error: la reduccion de prioridad se indica con 1/0 (si/no).\n");
        return 0;
    }
    return 1;
}

// se debe envíar una señal de terminación (SIGTERM) a todos los procesos hijos
void termHijos(int *buff, int l) {
    for (int i = 0; i < l; i++) {
        terminar(buff[i]);
    }
    for (int i = 0; i < l; i++) {
        wait(NULL);
    }
}

// signal 15 a un proceso
void terminar(int pid) {
    kill(pid, SIGTERM);
}
