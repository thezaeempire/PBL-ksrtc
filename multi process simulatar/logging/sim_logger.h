/* ===========================================================
 * sim_logger.h  -  ONE-FILE logging system (POSIX message queue)
 *
 * FOR UI / CORE (your friends):
 *     #include "sim_logger.h"
 *     log_open();                                    // once, at start
 *     log_send(LOG_INFO, SRC_CPU, "Executed ADD");   // anywhere
 *     log_close();                                   // at exit
 *   Build:  gcc -o core core.c -lrt -pthread
 *
 * FOR THE LOGGER PROCESS (same file, no extra .c needed):
 *     gcc -DSIM_LOGGER_MAIN -x c -o logger sim_logger.h -lrt -pthread
 *     ./logger            (start it FIRST; writes simulator.log)
 * =========================================================== */
#ifndef SIM_LOGGER_H
#define SIM_LOGGER_H

#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ---------- shared message format ---------- */
#define LOG_QUEUE_NAME  "/sim_log_queue"
#define LOG_QUEUE_DEPTH 10
#define LOG_TEXT_LEN    200

typedef enum { LOG_INFO = 0, LOG_WARN = 1, LOG_ERROR = 2, LOG_SHUTDOWN = 3 } log_level_t;
typedef enum { SRC_UI = 0, SRC_CORE, SRC_CPU, SRC_MEMORY, SRC_STACK, SRC_QUEUE } log_source_t;

typedef struct {
    int32_t level;
    int32_t source;
    int64_t ts_sec;
    int64_t ts_nsec;
    char    text[LOG_TEXT_LEN];
} log_msg_t;

/* ---------- client side: used by UI and Core ---------- */
static mqd_t g_log_mq = (mqd_t)-1;

static inline int log_open(void)
{
    g_log_mq = mq_open(LOG_QUEUE_NAME, O_WRONLY);
    if (g_log_mq == (mqd_t)-1) {
        perror("log_open (is ./logger running?)");
        return -1;
    }
    return 0;
}

static inline int log__send_msg(log_msg_t *m)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    m->ts_sec  = ts.tv_sec;
    m->ts_nsec = ts.tv_nsec;
    unsigned prio = (m->level == LOG_ERROR) ? 2 : (m->level == LOG_WARN) ? 1 : 0;
    if (mq_send(g_log_mq, (const char *)m, sizeof *m, prio) == -1) {
        perror("log_send");
        return -1;
    }
    return 0;
}

static inline int log_send(log_level_t lvl, log_source_t src, const char *fmt, ...)
    __attribute__((format(printf, 3, 4)));
static inline int log_send(log_level_t lvl, log_source_t src, const char *fmt, ...)
{
    log_msg_t m;
    memset(&m, 0, sizeof m);
    m.level  = lvl;
    m.source = src;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(m.text, sizeof m.text, fmt, ap);
    va_end(ap);
    return log__send_msg(&m);
}

static inline void log_shutdown(void)      /* asks the logger to stop */
{
    log_msg_t m;
    memset(&m, 0, sizeof m);
    m.level = LOG_SHUTDOWN;
    log__send_msg(&m);
}

static inline void log_close(void)
{
    if (g_log_mq != (mqd_t)-1) mq_close(g_log_mq);
    g_log_mq = (mqd_t)-1;
}

/* ---------- logger process (only compiled with -DSIM_LOGGER_MAIN) ---------- */
#ifdef SIM_LOGGER_MAIN

static volatile sig_atomic_t g_running = 1;
static void log__on_signal(int s) { (void)s; g_running = 0; }

int main(int argc, char **argv)
{
    static const char *LVL[] = { "INFO ", "WARN ", "ERROR" };
    static const char *SRC[] = { "UI", "CORE", "CPU", "MEM", "STACK", "QUEUE" };
    const char *path = (argc > 1) ? argv[1] : "simulator.log";

    FILE *fp = fopen(path, "a");
    if (!fp) { perror("fopen"); return 1; }
    signal(SIGINT,  log__on_signal);
    signal(SIGTERM, log__on_signal);

    struct mq_attr attr = {0};
    attr.mq_maxmsg  = LOG_QUEUE_DEPTH;
    attr.mq_msgsize = sizeof(log_msg_t);
    mq_unlink(LOG_QUEUE_NAME);
    mqd_t mq = mq_open(LOG_QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    if (mq == (mqd_t)-1) { perror("mq_open"); return 1; }
    printf("[logger] ready, writing to %s\n", path);

    long count = 0;
    double lat_sum = 0, lat_max = 0;
    log_msg_t m;
    unsigned prio;

    while (g_running) {
        struct timespec dl;
        clock_gettime(CLOCK_REALTIME, &dl);
        dl.tv_sec += 1;                       /* wake up to check g_running */
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

#endif /* SIM_LOGGER_MAIN */
#endif /* SIM_LOGGER_H */
