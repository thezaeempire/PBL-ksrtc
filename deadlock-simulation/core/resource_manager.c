#include "resource_manager.h"
#include "../algorithms/bankers.h"

ResourceResult resource_request(
    SystemState *state,
    int process_id,
    const int request[])
{
    if (state == NULL || request == NULL)
        return RESOURCE_INVALID;

    if (!state_validate(state))
        return RESOURCE_INVALID;

    if (process_id < 0 || process_id >= state->process_count)
        return RESOURCE_INVALID;

    if (state->terminated[process_id])
        return RESOURCE_INVALID;

    for (int j = 0; j < state->resource_count; j++) {
        int need = state_get_need(state, process_id, j);

        if (request[j] < 0 || request[j] > need)
            return RESOURCE_INVALID;

        if (request[j] > state->available[j])
            return RESOURCE_UNAVAILABLE;
    }

    /*
     * Banker’s algorithm temporarily allocates the resources,
     * checks safety, and rolls back if the state is unsafe.
     */
    int result = bankers_request(state, process_id, request);

    if (result == 1)
        return RESOURCE_GRANTED;

    if (result == 0)
        return RESOURCE_UNSAFE;

    return RESOURCE_INVALID;
}

int resource_release(
    SystemState *state,
    int process_id,
    const int release[])
{
    if (state == NULL || release == NULL)
        return 0;

    if (process_id < 0 || process_id >= state->process_count)
        return 0;

    if (state->terminated[process_id])
        return 0;

    for (int j = 0; j < state->resource_count; j++) {
        if (release[j] < 0 ||
            release[j] > state->allocation[process_id][j])
            return 0;
    }

    for (int j = 0; j < state->resource_count; j++) {
        state->allocation[process_id][j] -= release[j];
        state->available[j] += release[j];
    }

    return state_validate(state);
}
