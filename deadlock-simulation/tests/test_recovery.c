#include <assert.h>
#include <stdio.h>
#include "../core/system_state.h"
#include "../algorithms/recovery.h"

int main(void)
{
    SystemState state;
    state_init(&state);

    int available_before = state.available[0];
    int allocation_before = state.allocation[1][0];

    assert(recover_process(&state, 1) == 1);

    assert(state.terminated[1] == 1);
    assert(state.allocation[1][0] == 0);
    assert(state.available[0] ==
           available_before + allocation_before);

    /* A terminated process cannot be recovered again. */
    assert(recover_process(&state, 1) == 0);

    puts("PASS: recovery tests");
    return 0;
}
