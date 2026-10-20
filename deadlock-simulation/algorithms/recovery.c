#include "recovery.h"

int recover_process(SystemState *state, int process)
{
    if (!state_validate(state))
        return 0;

    if (process < 0 || process >= state->process_count || state->terminated[process])
        return 0;

    /*
     * Terminate the selected process and release its
     * allocated resources back to the system.
     */
    for (int j = 0; j < state->resource_count; j++) {
        state->allocation[process][j] = 0;
        state->maximum[process][j] = 0;
        state->request[process][j] = 0;
    }

    state->terminated[process] = 1;
    state_recalculate_available(state);

    return 1;
}