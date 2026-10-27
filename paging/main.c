#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page_replacement.h"

int main(void) {
    int refs[MAX_REFERENCES];
    int n = 0, capacity, choice;
    char line[10000];

    printf("PAGE REPLACEMENT SIMULATOR\n");
    printf("==========================\n");

    // Read page-reference string
    printf("Enter page references separated by spaces:\n");

    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }

    char *token = strtok(line, " \t\r\n");

    while (token != NULL) {
        char *end;
        long value = strtol(token, &end, 10);

        if (*end != '\0') {
            printf("Invalid page reference: %s\n", token);
            return 1;
        }

        if (n >= MAX_REFERENCES) {
            printf("Maximum %d page references allowed.\n",
                   MAX_REFERENCES);
            return 1;
        }

        refs[n++] = (int)value;
        token = strtok(NULL, " \t\r\n");
    }

    if (n == 0) {
        printf("Page-reference string cannot be empty.\n");
        return 1;
    }

    // Read and validate frame count
    printf("Enter number of frames (1-%d): ", MAX_FRAMES);

    if (scanf("%d", &capacity) != 1 ||
        capacity < 1 || capacity > MAX_FRAMES) {
        printf("Invalid number of frames.\n");
        return 1;
    }

    // Algorithm selection
    printf("\nSelect algorithm:\n");
    printf("1. FIFO\n");
    printf("2. LRU\n");
    printf("3. Optimal\n");
    printf("4. Compare all\n");
    printf("Choice: ");

    if (scanf("%d", &choice) != 1 ||
        choice < 1 || choice > 4) {
        printf("Invalid choice.\n");
        return 1;
    }

    Result fifo, lru, optimal;

    // Run the selected algorithm(s)
    if (choice == 1 || choice == 4) {
        fifo = simulate_fifo(refs, n, capacity);
    }

    if (choice == 2 || choice == 4) {
        lru = simulate_lru(refs, n, capacity);
    }

    if (choice == 3 || choice == 4) {
        optimal = simulate_optimal(refs, n, capacity);
    }

    // Display frame-by-frame results
    Result *results[] = {&fifo, &lru, &optimal};
    const char *names[] = {"FIFO", "LRU", "Optimal"};

    printf("\nTotal page references: %d\n", n);

    for (int a = 0; a < 3; a++) {
        if (choice != 4 && choice != a + 1) {
            continue;
        }

        Result *r = results[a];

        printf("\n--- %s ---\n", names[a]);
        printf("%-8s", "Page");

        for (int f = 0; f < capacity; f++) {
            printf("F%-9d", f + 1);
        }

        printf("Status\n");

        for (int i = 0; i < r->n; i++) {
            Step *s = &r->steps[i];

            printf("%-8d", s->page);

            for (int f = 0; f < capacity; f++) {
                if (s->frames[f] == EMPTY_FRAME) {
                    printf("%-10s", "-");
                } else {
                    printf("%-10d", s->frames[f]);
                }
            }

            printf("%s\n", s->status ? "Hit" : "Fault");
        }

        double hit_ratio = 100.0 * r->hits / n;
        double fault_ratio = 100.0 * r->faults / n;

        printf("Hits: %d | Faults: %d\n",
               r->hits, r->faults);

        printf("Hit ratio: %.1f%% | Fault ratio: %.1f%%\n",
               hit_ratio, fault_ratio);
    }

    // Comparative summary
    if (choice == 4) {
        printf("\n--- COMPARISON ---\n");
        printf("%-12s %-8s %-8s %-12s %-12s\n",
               "Algorithm", "Hits", "Faults",
               "Hit ratio", "Fault ratio");

        for (int a = 0; a < 3; a++) {
            Result *r = results[a];

            printf("%-12s %-8d %-8d %-11.1f%% %-11.1f%%\n",
                   names[a], r->hits, r->faults,
                   100.0 * r->hits / n,
                   100.0 * r->faults / n);
        }
    }

    return 0;
}
