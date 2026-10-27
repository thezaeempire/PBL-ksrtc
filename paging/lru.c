#include <stdio.h>
#include "page_replacement.h"

Result simulate_lru(const int refs[], int n, int capacity) {
    Result result = {0};

    int frames[MAX_FRAMES];
    int last_used[MAX_FRAMES];
    int count = 0;

    // Initialize frames and usage history
    for (int i = 0; i < capacity; i++) {
        frames[i] = EMPTY_FRAME;
        last_used[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        int page = refs[i];
        int frame_index = -1;

        // Check whether the page is already present
        for (int j = 0; j < count; j++) {
            if (frames[j] == page) {
                frame_index = j;
                break;
            }
        }

        if (frame_index != -1) {
            // Page hit
            result.hits++;
            result.steps[i].status = 1;
        } else {
            // Page fault
            result.faults++;
            result.steps[i].status = 0;

            if (count < capacity) {
                // Fill an empty frame
                frame_index = count;
                frames[count] = page;
                count++;
            } else {
                // Find the least recently used page
                int oldest = 0;

                for (int j = 1; j < count; j++) {
                    if (last_used[j] < last_used[oldest]) {
                        oldest = j;
                    }
                }

                frame_index = oldest;
                frames[frame_index] = page;
            }
        }

        // Update the most recent access time
        last_used[frame_index] = i;

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
