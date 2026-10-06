#ifndef LOG_MSG_H
#define LOG_MSG_H

#include <stdint.h>

/* POSIX message queue shared by all processes (UI, Core -> Logger) */
#define LOG_QUEUE_NAME  "/sim_log_queue"
#define LOG_QUEUE_DEPTH 10
#define LOG_TEXT_LEN    200

typedef enum { LOG_INFO = 0, LOG_WARN = 1, LOG_ERROR = 2, LOG_SHUTDOWN = 3 } log_level_t;

typedef enum {
    SRC_UI = 0, SRC_CORE, SRC_CPU, SRC_MEMORY, SRC_STACK, SRC_QUEUE
} log_source_t;

/* Fixed-size message: this is the "interface" every team member uses */
typedef struct {
    int32_t  level;                 /* log_level_t  */
    int32_t  source;                /* log_source_t */
    int64_t  ts_sec;                /* sender timestamp (CLOCK_REALTIME) */
    int64_t  ts_nsec;
    char     text[LOG_TEXT_LEN];
} log_msg_t;

#endif
