#include <stdio.h>
#include "page_replacement.h"

// Display frame-by-frame simulation results
void print_result(const char *name, const Result *result) {
    printf("\n===== %s ALGORITHM =====\n", name);

    printf("%-10s", "Page");

    for (int i = 0; i < result->capacity; i++) {
        printf("%-10s", "Frame");
    }

    printf("Status\n");

    for (int i = 0; i < result->n; i++) {
        const Step *step = &result->steps[i];

        printf("%-10d", step->page);

        for (int j = 0; j < result->capacity; j++) {
            if (step->frames[j] == EMPTY_FRAME) {
                printf("%-10s", "-");
            } else {
                printf("%-10d", step->frames[j]);
            }
        }

        printf("%s\n", step->status ? "Hit" : "Fault");
    }
}

// Calculate and display performance statistics
void print_summary(const char *name, const Result *result) {
    double hit_ratio = 0.0;
    double fault_ratio = 0.0;

    if (result->n > 0) {
        hit_ratio =
            (double)result->hits / result->n * 100.0;

        fault_ratio =
            (double)result->faults / result->n * 100.0;
    }

    printf("%-12s %-10d %-10d %-12.1f%% %-12.1f%%\n",
           name,
           result->hits,
           result->faults,
           hit_ratio,
           fault_ratio);
}
