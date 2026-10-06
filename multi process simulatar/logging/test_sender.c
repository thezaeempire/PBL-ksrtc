/* IPC test: acts like UI/Core and sends messages to the logger. */
#include <stdio.h>
#include <stdlib.h>
#include "log_client.h"

int main(int argc, char **argv)
{
    int burst = (argc > 1) ? atoi(argv[1]) : 1000;
    if (log_open() != 0) { fprintf(stderr, "Start ./logger first\n"); return 1; }

    /* Test 1: basic messages from different sources */
    log_send(LOG_INFO,  SRC_UI,     "User entered command: RUN program.asm");
    log_send(LOG_INFO,  SRC_CORE,   "Program loaded, %d instructions", 42);
    log_send(LOG_INFO,  SRC_CPU,    "Executed ADD R1, R2");
    log_send(LOG_INFO,  SRC_STACK,  "PUSH 15, sp=%d", 1);
    log_send(LOG_WARN,  SRC_MEMORY, "Memory usage at 90%%");
    log_send(LOG_ERROR, SRC_CPU,    "Illegal instruction at PC=0x%04X", 0x1A);

    /* Test 2: long text must be truncated safely */
    char longtxt[500];
    for (int i = 0; i < 499; i++) longtxt[i] = 'x';
    longtxt[499] = '\0';
    log_send(LOG_INFO, SRC_CORE, "%s", longtxt);

    /* Test 3: burst (queue-full / back-pressure + throughput) */
    for (int i = 0; i < burst; i++)
        log_send(LOG_INFO, SRC_QUEUE, "burst message %d", i);

    log_shutdown();
    log_close();
    printf("test_sender: sent %d burst messages\n", burst);
    return 0;
}
