
#include <stdio.h>
#include "page_replacement.h"

Result simulate_fifo(const int refs[], int n, int capacity) {
    Result result = {0};

    int frames[MAX_FRAMES];
    int count = 0;
    int next = 0;

    // Initialize all frames as empty
    for (int i = 0; i < capacity; i++) {
        frames[i] = EMPTY_FRAME;
    }

    // Process each page reference
    for (int i = 0; i < n; i++) {
        int page = refs[i];
        int found = 0;

        // Check whether the page is already in memory
        for (int j = 0; j < count; j++) {
            if (frames[j] == page) {
                found = 1;
                break;
            }
        }

        if (found) {
            // Page hit
            result.hits++;
            result.steps[i].status = 1;
        } else {
            // Page fault
            result.faults++;
            result.steps[i].status = 0;

            if (count < capacity) {
                // Use an empty frame first
                frames[count] = page;
                count++;
            } else {
                // Replace the oldest page
                frames[next] = page;
                next = (next + 1) % capacity;
            }
        }

        // Save the current page and frame state
        result.steps[i].page = page;

        for (int j = 0; j < capacity; j++) {
            result.steps[i].frames[j] =
                (j < count) ? frames[j] : EMPTY_FRAME;
        }
    }

    result.n = n;
    result.capacity = capacity;

    return result;
}
