#include "detection.h"

int detect_deadlock(
    const SystemState *state,
    int deadlocked[MAX_PROCESSES])
{
    int work[MAX_RESOURCES];
    int finished[MAX_PROCESSES] = {0};
    int count = 0;

    if (!state_validate(state))
        return -1;

    for (int j = 0; j < state->resource_count; j++)
        work[j] = state->available[j];

    /*
     * A process with no allocated resources can be treated
     * as finishable for this detection algorithm.
     */
    for (int i = 0; i < state->process_count; i++) {
        int has_allocation = 0;

        for (int j = 0; j < state->resource_count; j++) {
            if (state->allocation[i][j] > 0) {
                has_allocation = 1;
                break;
            }
        }

        finished[i] = !has_allocation;
    }

    int changed;

    do {
        changed = 0;

        for (int i = 0; i < state->process_count; i++) {
            if (finished[i])
                continue;

            int possible = 1;

            for (int j = 0; j < state->resource_count; j++) {
                if (state->request[i][j] > work[j]) {
                    possible = 0;
                    break;
                }
            }

            if (possible) {
                for (int j = 0; j < state->resource_count; j++)
                    work[j] += state->allocation[i][j];

                finished[i] = 1;
                changed = 1;
            }
        }
    } while (changed);

    for (int i = 0; i < state->process_count; i++) {
        deadlocked[i] = !finished[i];

        if (deadlocked[i])
            count++;
    }

    return count;
}