#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h> // times()
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>
int *buf;

// funcion invocada por los procesos hijos
int busywork(void)
{
    struct tms buf;
    for (;;)
    {
        times(&buf);
    }
}

void handler()
{
    int prioridad = getpriority(PRIO_PROCESS, 0);
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    int sum = usage.ru_utime.tv_sec + usage.ru_stime.tv_sec;

    printf("Child %d (nice %2d):\t%3li\n", getpid(), prioridad, sum);
    exit(0);
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = handler;
    if (valid(argc, argv) == 0)
        exit(EXIT_FAILURE);
    int _hijos = atoi(argv[1]);    // arg 1: numero procesos hijos
    int _segundos = atoi(argv[2]); // arg 2: tiempo que se debe ejecutar, 0 indefinido
    int _prio = atoi(argv[3]);     // arg 3: se reducen las prioridades (1?)
    int hijos[_hijos];             // arreglo con los pid de los procesos hijos

    buf = (int *)malloc(_hijos * sizeof(int));

    // iteracion fork, creacion de n hijos
    for (int i = 0; i < _hijos; i++)
    {
        pid_t child_pid = fork();
        if (child_pid < 0)
        {
            perror("error");
            exit(EXIT_FAILURE);
        }
        if (child_pid == 0)
        {

            sigaction(SIGTERM, &sa, NULL);
            if (_prio)
            {
                if (nice(i) == -1)
                {
                    perror("Error al asignar prioridad");
                }
            }
            busywork();
        }
        else
        {
            // hijos[i] = child_pid;
            buf[i] = child_pid;
        }
    }

    // implementar la ejecucion indefinida
    sleep(_segundos);

    // for (int i = 0; i < _hijos; i++)
    // {
    //     terminar(hijos[i]);
    // }
    // for (int i = 0; i < _hijos; i++)
    // {
    //     wait(NULL);
    // }
    // termHijos(hijos, sizeof(hijos) / sizeof(hijos[0]));
    termHijos(buf, _hijos);

    free(buf); // Libera la memoria
    exit(EXIT_SUCCESS);
}

// validar argumentos
int valid(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <num_hijos> <segundos_ejecucion> <reduccion_prioridad>\n", argv[0]);
        return 0;
    }
    int h = atoi(argv[1]);
    int s = atoi(argv[2]);
    int r = atoi(argv[3]);

    if (h < 0 || s < 0)
        return 0;
    else if (r != 0 && r != 1)
        return 0;

    return 1;
}

// se debe envíar una señal de terminación (SIGTERM) a todos los procesos hijos
void termHijos(int *hijos, int l)
{
    for (int i = 0; i < l; i++)
    {
        terminar(hijos[i]);
    }
    for (int i = 0; i < l; i++)
    {
        wait(NULL);
    }
}

// signal 15 a un proceso
void terminar(int pid)
{
    kill(pid, SIGTERM);
}
