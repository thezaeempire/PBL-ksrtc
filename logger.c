/* Logging Process: receives log messages over a POSIX message queue and
 * writes them to simulator.log (and stdout). Also measures IPC latency. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "log_msg.h"

static volatile sig_atomic_t running = 1;
static void on_signal(int s) { (void)s; running = 0; }

static const char *LVL[] = { "INFO ", "WARN ", "ERROR" };
static const char *SRC[] = { "UI", "CORE", "CPU", "MEM", "STACK", "QUEUE" };

int main(int argc, char **argv)
{
    const char *path = (argc > 1) ? argv[1] : "simulator.log";
    FILE *fp = fopen(path, "a");
    if (!fp) { perror("fopen"); return 1; }

    signal(SIGINT, on_signal);
    signal(SIGTERM, on_signal);

    struct mq_attr attr = {0};
    attr.mq_maxmsg  = LOG_QUEUE_DEPTH;
    attr.mq_msgsize = sizeof(log_msg_t);

    mq_unlink(LOG_QUEUE_NAME);                      /* start clean */
    mqd_t mq = mq_open(LOG_QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    if (mq == (mqd_t)-1) { perror("mq_open"); return 1; }

    printf("[logger] ready, writing to %s\n", path);

    long   count = 0;
    double lat_sum = 0, lat_max = 0;
    log_msg_t m;
    unsigned prio;

    while (running) {
        struct timespec dl;
        clock_gettime(CLOCK_REALTIME, &dl);
        dl.tv_sec += 1;                              /* wake up to check 'running' */
        ssize_t n = mq_timedreceive(mq, (char *)&m, sizeof m, &prio, &dl);
        if (n < 0) {
            if (errno == ETIMEDOUT || errno == EINTR) continue;
            perror("mq_timedreceive");
            break;
        }
        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);

        if (m.level == LOG_SHUTDOWN) break;
        if (m.level < 0 || m.level > LOG_ERROR) m.level = LOG_INFO;
        if (m.source < 0 || m.source > SRC_QUEUE) m.source = SRC_CORE;
        m.text[LOG_TEXT_LEN - 1] = '\0';

        /* IPC latency = receive time - send time (microseconds) */
        double lat = (now.tv_sec - m.ts_sec) * 1e6 + (now.tv_nsec - m.ts_nsec) / 1e3;
        lat_sum += lat;
        if (lat > lat_max) lat_max = lat;
        count++;

        char tbuf[32];
        struct tm tmv;
        time_t t = (time_t)m.ts_sec;
        localtime_r(&t, &tmv);
        strftime(tbuf, sizeof tbuf, "%Y-%m-%d %H:%M:%S", &tmv);

        fprintf(fp, "[%s.%03ld] [%s] [%-5s] %s\n", tbuf, (long)(m.ts_nsec / 1000000),
                LVL[m.level], SRC[m.source], m.text);
        fflush(fp);
        printf("[%s] [%-5s] %s\n", LVL[m.level], SRC[m.source], m.text);
    }

    printf("[logger] stopped. messages=%ld avg_latency=%.1f us max_latency=%.1f us\n",
           count, count ? lat_sum / count : 0.0, lat_max);
    fprintf(fp, "# logger stopped: messages=%ld avg_latency_us=%.1f max_latency_us=%.1f\n",
            count, count ? lat_sum / count : 0.0, lat_max);

    fclose(fp);
    mq_close(mq);
    mq_unlink(LOG_QUEUE_NAME);
    return 0;
}
