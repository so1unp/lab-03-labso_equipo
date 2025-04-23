#include "sigcatch.h"

void handler(int sig)
{
    printf("%d: %s\n", sig, strsignal(sig));
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = handler;

    // usar siguientes lineas para probar:
    //  kill -8 $(ps | grep sigcatch | awk '{print $1}')
    //  kill -15 $(ps | grep sigcatch | awk '{print $1}')
    for (size_t i = 0; i < _NSIG; i++)
    {
        sigaction(i, &sa, NULL);
    }

    while (1)
    {
        pause(); // espera signal infinitamente
    }

    exit(EXIT_SUCCESS);
}