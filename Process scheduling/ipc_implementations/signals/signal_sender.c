
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

int main(int argc, char *argv[])
{
    pid_t receiver_pid;
    char *endptr;

    if (argc != 2) {
        printf("Usage: %s <receiver_PID>\n", argv[0]);
        return 1;
    }

    errno = 0;
    long value = strtol(argv[1], &endptr, 10);

    if (errno != 0 || *endptr != '\0' ||
        value <= 0 || value > INT_MAX) {
        fprintf(stderr, "Invalid PID.\n");
        return 1;
    }

    receiver_pid = (pid_t)value;

    if (kill(receiver_pid, SIGUSR1) == -1) {
        perror("kill");
        return 1;
    }

    printf("Sender: SIGUSR1 sent to PID %ld\n", value);

    return 0;
}
