#include <stdio.h>
#include "scheduling.h"

/*
 * PERSON 4: ROUND ROBIN SCHEDULING
 *
 * Each process receives CPU time up to the time quantum.
 * If it is unfinished, it returns to the ready queue.
 */

void round_robin(Process input[], int n, int quantum) {
    Process p[MAX];
    Segment g[MAX_SEGMENTS];

    int order[MAX];
    int queue[MAX + 1];
    int enqueued[MAX] = {0};

    int head = 0, tail = 0;
    int count = 0;
    int completed = 0;
    int time = 0;

    if (n < 1 || n > MAX || quantum <= 0) {
        printf("Invalid process count or time quantum.\n");
        return;
    }

    /* Copy processes and initialize their state */
    for (int i = 0; i < n; i++) {
        p[i] = input[i];
        p[i].remaining = p[i].burst;
        p[i].start = -1;
        p[i].completion = 0;
        order[i] = i;
    }

    /* Sort process indices by arrival time */
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[order[j]].arrival >
                p[order[j + 1]].arrival) {
                int temp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = temp;
            }
        }
    }

    int next = 0;

    while (completed < n) {

        /* If the queue is empty, advance to the next arrival */
        if (head == tail) {
            if (next < n && time < p[order[next]].arrival) {
                add_segment(g, &count, -1,
                            time, p[order[next]].arrival);

                time = p[order[next]].arrival;
            }

            /* Add all processes that have arrived */
            while (next < n &&
                   p[order[next]].arrival <= time) {
                int idx = order[next++];

                if (p[idx].remaining > 0 && !enqueued[idx]) {
                    queue[tail] = idx;
                    tail = (tail + 1) % (MAX + 1);
                    enqueued[idx] = 1;
                }
            }

            if (head == tail)
                continue;
        }

        /* Select the process at the front of the queue */
        int current = queue[head];
        head = (head + 1) % (MAX + 1);
        enqueued[current] = 0;

        /* Record the first CPU start time */
        if (p[current].start == -1)
            p[current].start = time;

        /* Run for one quantum or until completion */
        int run_time = p[current].remaining < quantum
                     ? p[current].remaining : quantum;

        add_segment(g, &count, p[current].id,
                    time, time + run_time);

        time += run_time;
        p[current].remaining -= run_time;

        /* Enqueue arrivals during this time slice first */
        while (next < n &&
               p[order[next]].arrival <= time) {
            int idx = order[next++];

            if (p[idx].remaining > 0 && !enqueued[idx]) {
                queue[tail] = idx;
                tail = (tail + 1) % (MAX + 1);
                enqueued[idx] = 1;
            }
        }

        /* Finish or requeue the running process */
        if (p[current].remaining == 0) {
            p[current].completion = time;
            completed++;
        } else {
            queue[tail] = current;
            tail = (tail + 1) % (MAX + 1);
            enqueued[current] = 1;
        }
    }

    printf("\n=== Round Robin Scheduling ===\n");
    printf("Time Quantum: %d\n", quantum);

    print_results(p, n, g, count);
}
