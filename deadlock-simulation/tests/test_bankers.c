#include <assert.h>
#include <stdio.h>
#include "../core/system_state.h"
#include "../algorithms/bankers.h"

int main(void)
{
    SystemState state;
    int sequence[MAX_PROCESSES];
    int length = 0;

    state_init(&state);

    assert(bankers_safety(&state, sequence, &length) == 1);
    assert(length == 5);

    /* P0's request (0, 2, 0) is valid and safe. */
    int request[] = {0, 2, 0};

    assert(bankers_request(&state, 0, request) == 1);
    assert(state.allocation[0][1] == 3);
    assert(state.available[1] == 1);

    puts("PASS: Banker's algorithm tests");
    return 0;
}


    state_init(&state);
