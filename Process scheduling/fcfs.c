#include <stdio.h>
#include "scheduling.h"

void fcfs_schedule(Process input[], int n)
{
    Process p[MAX];
    Segment g[MAX_SEGMENTS];
    int count = 0;
    int time = 0;

    if (n < 1 || n > MAX) {
        printf("Invalid number of processes.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        p[i] = input[i];
    }

    /* Sort by arrival time, keeping input order for ties. */
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival > p[j + 1].arrival) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival) {
            add_segment(g, &count, -1, time, p[i].arrival);
            time = p[i].arrival;
        }

        p[i].start = time;
        p[i].response = time - p[i].arrival;
        p[i].waiting = time - p[i].arrival;

        add_segment(g, &count, p[i].id, time, time + p[i].burst);

        time += p[i].burst;
        p[i].completion = time;
        p[i].turnaround = time - p[i].arrival;
    }

    printf("\n=== FCFS Scheduling ===\n");
    print_results(p, n, g, count);
}
