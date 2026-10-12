
#include <stdio.h>
#include "scheduling.h"

/*
 * PERSON 3: PRIORITY SCHEDULING
 *
 * Smaller priority number = higher priority.
 * Uses shared add_segment() and print_results()
 * functions implemented by Person 5.
 */


/* 1. NON-PREEMPTIVE PRIORITY SCHEDULING */
void priority_non_preemptive(Process input[], int n) {
    Process p[MAX];
    Segment g[MAX_SEGMENTS];

    int done[MAX] = {0};
    int count = 0;
    int completed = 0;
    int time = 0;

    /* Copy processes */
    for (int i = 0; i < n; i++) {
        p[i] = input[i];
        p[i].start = -1;
        p[i].completion = 0;
    }

    while (completed < n) {
        int best = -1;

        /* Find highest-priority arrived process */
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].arrival <= time) {

                if (best == -1 ||
                    p[i].priority < p[best].priority ||
                    (p[i].priority == p[best].priority &&
                     p[i].arrival < p[best].arrival)) {
                    best = i;
                }
            }
        }

        /* No process has arrived yet */
        if (best == -1) {
            add_segment(g, &count, -1, time, time + 1);
            time++;
            continue;
        }

        /* Execute the selected process completely */
        p[best].start = time;

        add_segment(g, &count, p[best].id,
                    time, time + p[best].burst);

        time += p[best].burst;
        p[best].completion = time;

        done[best] = 1;
        completed++;
    }

    printf("\n=== Non-Preemptive Priority Scheduling ===\n");

    print_results(p, n, g, count);
}


/* 2. PREEMPTIVE PRIORITY SCHEDULING */
void priority_preemptive(Process input[], int n) {
    Process p[MAX];
    Segment g[MAX_SEGMENTS];

    int count = 0;
    int completed = 0;
    int time = 0;

    /* Initialize remaining burst time */
    for (int i = 0; i < n; i++) {
        p[i] = input[i];
        p[i].remaining = p[i].burst;
        p[i].start = -1;
        p[i].completion = 0;
    }

    while (completed < n) {
        int best = -1;

        /* Find highest-priority ready process */
        for (int i = 0; i < n; i++) {
            if (p[i].remaining > 0 &&
                p[i].arrival <= time) {

                if (best == -1 ||
                    p[i].priority < p[best].priority ||
                    (p[i].priority == p[best].priority &&
                     p[i].arrival < p[best].arrival)) {
                    best = i;
                }
            }
        }

        /* CPU is idle */
        if (best == -1) {
            add_segment(g, &count, -1, time, time + 1);
            time++;
            continue;
        }

        /* Record the first time the process runs */
        if (p[best].start == -1) {
            p[best].start = time;
        }

        /* Execute for one time unit */
        add_segment(g, &count, p[best].id,
                    time, time + 1);

        p[best].remaining--;
        time++;

        /* Check whether the process has finished */
        if (p[best].remaining == 0) {
            p[best].completion = time;
            completed++;
        }
    }

    printf("\n=== Preemptive Priority Scheduling ===\n");

    print_results(p, n, g, count);
}

