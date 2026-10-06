#ifndef LOG_CLIENT_H
#define LOG_CLIENT_H
#include "log_msg.h"

/* Call from UI / Core processes */
int  log_open(void);                                   /* 0 = ok, -1 = fail */
int  log_send(log_level_t lvl, log_source_t src, const char *fmt, ...)
         __attribute__((format(printf, 3, 4)));
void log_shutdown(void);                               /* tells logger to exit */
void log_close(void);

#endif
