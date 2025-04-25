#include "sigcatch.h"

void handler(int sig)
{
    printf("%d: %s\n", sig, strsignal(sig));
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = handler;
    for (int i = 1; i < _NSIG; i++)
    {
        sigaction(i, &sa, NULL);
    }
    while (1)
    {
        pause();
    }
    exit(EXIT_SUCCESS);
}