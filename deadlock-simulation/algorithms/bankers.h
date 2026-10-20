#ifndef BANKERS_H
#define BANKERS_H

#include "../system_state.h"

int bankers_safety(
    const SystemState *state,
    int safe_sequence[MAX_PROCESSES]
);

int bankers_request(
    SystemState *state,
    int process,
    const int request[MAX_RESOURCES]
);

#endif