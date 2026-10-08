#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t core, ui, logger;

    printf("=== MULTI PROCESS SIMULATOR ===\n");

    logger = fork();

    if (logger == 0)
    {
        execl("./logging/logger", "logger", NULL);
        perror("Logger failed");
        exit(1);
    }

         sleep(1);   
         core = fork();

    if (core == 0)
    {
        execl("./core/core", "core", NULL);
        perror("Core failed");
        exit(1);
    }

    ui = fork();

    if (ui == 0)
    {
        execl("./ui/ui", "ui", NULL);
        perror("UI failed");
        exit(1);
    }

    printf("Logger PID : %d\n", logger);
    printf("Core PID   : %d\n", core);
    printf("UI PID     : %d\n", ui);

    wait(NULL);
    wait(NULL);
    wait(NULL);

    printf("Simulator shutdown complete.\n");

    return 0;
}
