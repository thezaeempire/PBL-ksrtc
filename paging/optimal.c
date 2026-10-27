#include <stdio.h>
#include "page_replacement.h"

Result simulate_optimal(const int refs[], int n, int capacity) {
    Result result = {0};

    int frames[MAX_FRAMES];
    int count = 0;

    // Initialize frames as empty
    for (int i = 0; i < capacity; i++) {
        frames[i] = EMPTY_FRAME;
    }

    for (int i = 0; i < n; i++) {
        int page = refs[i];
        int found = 0;

        // Check whether the page is already present
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
                // Fill an empty frame
                frames[count] = page;
                count++;
            } else {
                // Find the page used farthest in the future
                int victim = 0;
                int farthest = -1;

                for (int j = 0; j < count; j++) {
                    int next_use = n + 1;

                    for (int k = i + 1; k < n; k++) {
                        if (refs[k] == frames[j]) {
                            next_use = k;
                            break;
                        }
                    }

                    if (next_use > farthest) {
                        farthest = next_use;
                        victim = j;
                    }
                }

                // Replace the selected page
                frames[victim] = page;
            }
        }

        // Save the current simulation state
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