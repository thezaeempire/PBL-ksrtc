#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "system_state.h"

typedef enum {
    RESOURCE_GRANTED = 1,
    RESOURCE_UNAVAILABLE = 0,
    RESOURCE_INVALID = -1,
    RESOURCE_UNSAFE = -2
} ResourceResult;

ResourceResult resource_request(
    SystemState *state,
    int process_id,
    const int request[]
);

int resource_release(
    SystemState *state,
    int process_id,
    const int release[]
);

#endif
