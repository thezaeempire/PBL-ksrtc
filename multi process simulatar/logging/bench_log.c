/* Benchmark: measures IPC send cost for the logger. Usage: ./bench_log [N] */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include "log_client.h"

static double now_s(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1e9;
}

int main(int argc, char **argv)
{
    int n = (argc > 1) ? atoi(argv[1]) : 10000;
    if (log_open() != 0) { fprintf(stderr, "Start ./logger first\n"); return 1; }

    double t0 = now_s();
    for (int i = 0; i < n; i++)
        log_send(LOG_INFO, SRC_CPU, "bench message %d", i);
    double t1 = now_s();

    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);

    printf("messages sent     : %d\n", n);
    printf("total time        : %.4f s\n", t1 - t0);
    printf("avg send time     : %.2f us/message\n", (t1 - t0) * 1e6 / n);
    printf("throughput        : %.0f messages/s\n", n / (t1 - t0));
    printf("sender CPU time   : %.4f s\n",
           ru.ru_utime.tv_sec + ru.ru_utime.tv_usec / 1e6 +
           ru.ru_stime.tv_sec + ru.ru_stime.tv_usec / 1e6);
    printf("sender max memory : %ld KB\n", ru.ru_maxrss);

    log_shutdown();
    log_close();
    return 0;
}
