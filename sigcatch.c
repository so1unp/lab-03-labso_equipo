#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
//

void handler(int sig)
{
    printf("%d: %s\n", sig, strsignal(sig));
}
int main(int argc, char *argv[])
{
    // refactoring: implementar con sigaction
    signal(8, handler);
    signal(15, handler);

    while (1) {
        pause();  // espera señales infinitamente
    }

    exit(EXIT_SUCCESS);
}