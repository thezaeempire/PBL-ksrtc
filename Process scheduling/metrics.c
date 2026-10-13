#include <stdio.h>
#include "scheduling.h"

/* Add an execution interval to the Gantt chart.
   Merge consecutive intervals belonging to the same process. */
void add_segment(Segment g[], int *count,
                 int pid, int start, int end) {
    if (end <= start)
        return;

    if (*count > 0 &&
        g[*count - 1].pid == pid &&
        g[*count - 1].end == start) {
        g[*count - 1].end = end;
        return;
    }

    if (*count >= MAX_SEGMENTS) {
        printf("Error: Gantt chart capacity exceeded.\n");
        return;
    }

    g[*count].pid = pid;
    g[*count].start = start;
    g[*count].end = end;
    (*count)++;
}

/* Display Gantt chart and scheduling performance metrics. */
void print_results(Process p[], int n,
                   Segment g[], int segments) {
    double totalWT = 0.0;
    double totalTAT = 0.0;
    int busyTime = 0;

    if (n <= 0 || segments <= 0) {
        printf("No scheduling results available.\n");
        return;
    }

    printf("\n========== GANTT CHART ==========\n");

    for (int i = 0; i < segments; i++) {
        if (g[i].pid == -1)
            printf("| Idle ");
        else
            printf("| P%d ", g[i].pid);
    }
    printf("|\n");

    printf("%d", g[0].start);
    for (int i = 0; i < segments; i++)
        printf("\t%d", g[i].end);
    printf("\n");

    printf("\n========== PROCESS METRICS ==========\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\tRT\n");

    for (int i = 0; i < n; i++) {
        p[i].turnaround = p[i].completion - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;
        p[i].response = p[i].start - p[i].arrival;

        totalWT += p[i].waiting;
        totalTAT += p[i].turnaround;

        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id,
               p[i].arrival,
               p[i].burst,
               p[i].completion,
               p[i].turnaround,
               p[i].waiting,
               p[i].response);
    }

    printf("\nAverage Waiting Time: %.2f\n",
           totalWT / n);

    printf("Average Turnaround Time: %.2f\n",
           totalTAT / n);

    for (int i = 0; i < segments; i++) {
        if (g[i].pid != -1)
            busyTime += g[i].end - g[i].start;
    }

    int totalTime = g[segments - 1].end - g[0].start;

    if (totalTime > 0) {
        printf("CPU Utilization: %.2f%%\n",
               100.0 * busyTime / totalTime);
    }

    printf("====================================\n");
}
