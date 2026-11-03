
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t signal_received = 0;

void handle_signal(int sig)
{
    if (sig == SIGUSR1) {
        signal_received = 1;
    }
}

int main()
{
    struct sigaction sa;

    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    printf("Receiver: PID = %d\n", getpid());
    printf("Receiver: Waiting for SIGUSR1...\n");
    fflush(stdout);

    while (!signal_received) {
        pause();
    }

    printf("Receiver: SIGUSR1 received successfully!\n");
    printf("Receiver: Signal handled.\n");

    return 0;
}
