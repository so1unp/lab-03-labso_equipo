#include "sigcatch.h"

void handler(int sig)
{
    printf("%d: %s\n", sig, strsignal(sig));
}

int main(int argc, char *argv[])
{
    // refactoring: implementar con sigaction
    // signal(SIGTERM, handler);
    // signal(SIGFPE, handler);
    struct sigaction sa;
    sa.sa_handler = handler;

    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGFPE, &sa, NULL); // averiguar porque null

    while (1)
    {
        pause(); // espera signal infinitamente
    }

    exit(EXIT_SUCCESS);
}