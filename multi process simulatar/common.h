#ifndef COMMON_H
#define COMMON_H

#define UI_TO_CORE "/ui_to_core"
#define CORE_TO_LOG "/core_to_log"

#define MAX_MSG 256

typedef struct
{
    int number;
    char operation[20];
} CoreMessage;

typedef struct
{
    int result;
    char message[MAX_MSG];
} LogMessage;

#endif