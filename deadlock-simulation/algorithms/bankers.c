#include <stddef.h>
#include "bankers.h"

int bankers_safety(
    const SystemState *state,
    int safe_sequence[MAX_PROCESSES], int *length)
{
    int need[MAX_PROCESSES][MAX_RESOURCES];
    int work[MAX_RESOURCES];
    int finished[MAX_PROCESSES] = {0};
    int count = 0;

    if (length != NULL) *length = 0;
    if (state == NULL || safe_sequence == NULL || length == NULL ||
        !state_validate(state))
        return 0;

    /* Calculate the Need matrix. */
    for (int i = 0; i < state->process_count; i++) {
        for (int j = 0; j < state->resource_count; j++) {
            need[i][j] = state_get_need(state, i, j);
        }
    }

    /* Initialize Work with Available resources. */
    for (int j = 0; j < state->resource_count; j++) {
        work[j] = state->available[j];
    }

    /* Find a safe execution sequence. */
    while (count < state->process_count) {
        int found = 0;

        for (int i = 0; i < state->process_count; i++) {
            if (finished[i])
                continue;

            int possible = 1;

            for (int j = 0; j < state->resource_count; j++) {
                if (need[i][j] > work[j]) {
                    possible = 0;
                    break;
                }
            }

            if (possible) {
                for (int j = 0; j < state->resource_count; j++) {
                    work[j] += state->allocation[i][j];
                }

                finished[i] = 1;
                safe_sequence[count++] = i;
                found = 1;
            }
        }

        if (!found)
            return 0;
    }

    *length = count;
    return 1;
}

int bankers_request(
    SystemState *state,
    int process,
    const int request[MAX_RESOURCES])
{
    int need[MAX_PROCESSES][MAX_RESOURCES];
    int sequence[MAX_PROCESSES];

    if (state == NULL || request == NULL ||
        !state_validate(state))
        return -1;

    if (process < 0 || process >= state->process_count)
        return -1;

    /* Calculate the Need matrix. */
    for (int i = 0; i < state->process_count; i++) {
        for (int j = 0; j < state->resource_count; j++) {
            need[i][j] = state_get_need(state, i, j);
        }
    }

    /* Validate the requested resources. */
    for (int j = 0; j < state->resource_count; j++) {
        if (request[j] < 0 || request[j] > need[process][j])
            return -1;

        if (request[j] > state->available[j])
            return 0;
    }

    /* Tentatively allocate resources. */
    for (int j = 0; j < state->resource_count; j++) {
        state->available[j] -= request[j];
        state->allocation[process][j] += request[j];
    }

    /* Keep the allocation only if the state is safe. */
    int length = 0;
    if (bankers_safety(state, sequence, &length)) {
        return 1;
    }

    /* Roll back an unsafe allocation. */
    for (int j = 0; j < state->resource_count; j++) {
        state->available[j] += request[j];
        state->allocation[process][j] -= request[j];
    }

    return 0;
}
