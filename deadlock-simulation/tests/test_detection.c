#include <assert.h>
#include <stdio.h>
#include "../core/system_state.h"
#include "../algorithms/detection.h"

int main(void)
{
    SystemState state;
    int deadlocked[MAX_PROCESSES] = {0};

    state_init(&state);

    /* Construct a two-process circular wait using R0 and R1. */
    state.process_count = 2;
    state.resource_count = 2;

    state.total[0] = 1;
    state.total[1] = 1;

    state.allocation[0][0] = 1;
    state.allocation[0][1] = 0;

    state.allocation[1][0] = 0;
    state.allocation[1][1] = 1;

    state.request[0][0] = 0;
    state.request[0][1] = 1;

    state.request[1][0] = 1;
    state.request[1][1] = 0;

    state_recalculate_available(&state);

    int count = detect_deadlock(&state, deadlocked);

    assert(count == 2);
    assert(deadlocked[0] == 0);
    assert(deadlocked[1] == 1);

    puts("PASS: deadlock detection tests");
    return 0;
}

#include "../core/system_state.h"
