#include "scheduling.h"
#include <stdio.h>

void sjf_non_preemptive(Process input[], int n) {
    Process p[MAX];
    Segment g[MAX_SEGMENTS];
    int done[MAX] = {0};
    int count = 0, completed = 0, time = 0;

    for (int i = 0; i < n; i++) {
        p[i] = input[i];
        p[i].start = -1;
        p[i].completion = 0;
    }

    while (completed < n) {
        int best = -1;

        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].arrival <= time) {
                if (best == -1 ||
                    p[i].burst < p[best].burst ||
                    (p[i].burst == p[best].burst &&
                     p[i].arrival < p[best].arrival)) {
                    best = i;
                }
            }
        }

        if (best == -1) {
            add_segment(g, &count, -1, time, time + 1);
            time++;
            continue;
        }

        p[best].start = time;
        add_segment(g, &count, p[best].id,
                    time, time + p[best].burst);

        time += p[best].burst;
        p[best].completion = time;
        done[best] = 1;
        completed++;
    }

    printf("\n=== Non-Preemptive SJF ===\n");
    print_results(p, n, g, count);
}

void sjf_preemptive(Process input[], int n) {
    Process p[MAX];
    Segment g[MAX_SEGMENTS];
    int count = 0, completed = 0, time = 0;

    for (int i = 0; i < n; i++) {
        p[i] = input[i];
        p[i].remaining = p[i].burst;
        p[i].start = -1;
        p[i].completion = 0;
    }

    while (completed < n) {
        int best = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].remaining > 0 &&
                p[i].arrival <= time) {
                if (best == -1 ||
                    p[i].remaining < p[best].remaining ||
                    (p[i].remaining == p[best].remaining &&
                     p[i].arrival < p[best].arrival)) {
                    best = i;
                }
            }
        }

        if (best == -1) {
            add_segment(g, &count, -1, time, time + 1);
            time++;
            continue;
        }

        if (p[best].start == -1)
            p[best].start = time;

        add_segment(g, &count, p[best].id,
                    time, time + 1);

        p[best].remaining--;
        time++;

        if (p[best].remaining == 0) {
            p[best].completion = time;
            completed++;
        }
    }

    printf("\n=== Preemptive SJF (SRTF) ===\n");
    print_results(p, n, g, count);
}

