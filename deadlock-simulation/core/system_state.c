#include "system_state.h"

#include <string.h>

void state_init(SystemState *state)
{
    if (state == NULL)
        return;

    memset(state, 0, sizeof(SystemState));

    state->process_count = 5;
    state->resource_count = 3;

    for (int i = 0; i < state->process_count; i++) {
        strcpy(state->process_names[i], "P0");
        state->process_names[i][1] = '0' + i;
    }

    for (int j = 0; j < state->resource_count; j++) {
        strcpy(state->resource_names[j], "R0");
        state->resource_names[j][1] = '0' + j;
    }

    /* Total instances of resources R0, R1 and R2. */
    state->total[0] = 10;
    state->total[1] = 5;
    state->total[2] = 7;

    int allocation[5][3] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    int maximum[5][3] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    memcpy(state->allocation, allocation, sizeof(allocation));
    memcpy(state->maximum, maximum, sizeof(maximum));

    state_recalculate_available(state);
}

void state_recalculate_available(SystemState *state)
{
    if (state == NULL)
        return;

    for (int j = 0; j < state->resource_count; j++) {
        int allocated = 0;

        for (int i = 0; i < state->process_count; i++)
            allocated += state->allocation[i][j];

        state->available[j] = state->total[j] - allocated;
    }
}

int state_get_need(
    const SystemState *state,
    int process_id,
    int resource_id)
{
    if (state == NULL)
        return -1;

    if (process_id < 0 || process_id >= state->process_count)
        return -1;

    if (resource_id < 0 || resource_id >= state->resource_count)
        return -1;

    return state->maximum[process_id][resource_id]
         - state->allocation[process_id][resource_id];
}

int state_validate(const SystemState *state)
{
    if (state == NULL)
        return 0;

    if (state->process_count < 1 ||
        state->process_count > MAX_PROCESSES)
        return 0;

    if (state->resource_count < 1 ||
        state->resource_count > MAX_RESOURCES)
        return 0;

    for (int j = 0; j < state->resource_count; j++) {
        if (state->total[j] < 0)
            return 0;
    }

    for (int i = 0; i < state->process_count; i++) {
        for (int j = 0; j < state->resource_count; j++) {
            if (state->allocation[i][j] < 0)
                return 0;

            if (state->maximum[i][j] < state->allocation[i][j])
                return 0;

            if (state->request[i][j] < 0)
                return 0;
        }
    }

    for (int j = 0; j < state->resource_count; j++) {
        int allocated = 0;

        for (int i = 0; i < state->process_count; i++)
            allocated += state->allocation[i][j];

        if (allocated > state->total[j])
            return 0;
    }

    return 1;
}
