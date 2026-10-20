#ifndef DETECTION_H
#define DETECTION_H

#include "../system_state.h"

int detect_deadlock(
    const SystemState *state,
    int deadlocked[MAX_PROCESSES]
);

#endif