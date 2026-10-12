#include <stdio.h>

#define MAX 100
#define MAX_SEGMENTS 100000

typedef struct {
    int id;
    int arrival;
    int burst;
    int priority;
    int remaining;
    int start;
    int completion;
    int waiting;
    int turnaround;
    int response;
} Process;

typedef struct {
    int pid;
    int start;
    int end;
} Segment;

void print_results(Process p[], int n,
                   Segment g[], int segments) {
    double totalWT = 0, totalTAT = 0;
    int first = 0, last = 0;

    printf("\nGantt Chart:\n");
    for (int i = 0; i < segments; i++) {
        printf("| %s%d ",
               g[i].pid == -1 ? "Idle" : "P",
               g[i].pid == -1 ? 0 : g[i].pid);
    }
    printf("|\n");

    if (segments > 0) {
        printf("%d", g[0].start);
        for (int i = 0; i < segments; i++)
            printf("\t%d", g[i].end);
        printf("\n");

        first = g[0].start;
        last = g[segments - 1].end;
    }

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\tRT\n");

    for (int i = 0; i < n; i++) {
        p[i].turnaround = p[i].completion - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;
        p[i].response = p[i].start - p[i].arrival;

        totalWT += p[i].waiting;
        totalTAT += p[i].turnaround;

        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].arrival, p[i].burst,
               p[i].completion, p[i].turnaround,
               p[i].waiting, p[i].response);
    }

    printf("\nAverage Waiting Time: %.2f\n", totalWT / n);
    printf("Average Turnaround Time: %.2f\n", totalTAT / n);

    if (last > first) {
        int busy = 0;
        for (int i = 0; i < segments; i++)
            if (g[i].pid != -1)
                busy += g[i].end - g[i].start;

        printf("CPU Utilization: %.2f%%\n",
               100.0 * busy / (last - first));
    }
}

void add_segment(Segment g[], int *count,
                 int pid, int start, int end) {
    if (start == end)
        return;

    if (*count > 0 &&
        g[*count - 1].pid == pid &&
        g[*count - 1].end == start) {
        g[*count - 1].end = end;
        return;
    }

    if (*count < MAX_SEGMENTS) {
        g[*count].pid = pid;
        g[*count].start = start;
        g[*count].end = end;
        (*count)++;
    }
}

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

int main(void) {
    Process p[MAX];
    int n, choice;

    printf("=== SJF Scheduling Simulator ===\n");
    printf("Number of processes (1-%d): ", MAX);

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("\nProcess %d\n", i + 1);
        printf("ID: ");

        if (scanf("%d", &p[i].id) != 1)
            return 1;

        for (int j = 0; j < i; j++) {
            if (p[j].id == p[i].id) {
                printf("Process IDs must be unique.\n");
                return 1;
            }
        }

        printf("Arrival Time: ");
        if (scanf("%d", &p[i].arrival) != 1 ||
            p[i].arrival < 0) {
            printf("Arrival time must be non-negative.\n");
            return 1;
        }

        printf("Burst Time: ");
        if (scanf("%d", &p[i].burst) != 1 ||
            p[i].burst <= 0) {
            printf("Burst time must be positive.\n");
            return 1;
        }

        p[i].priority = 0;
    }

    printf("\n1. Non-Preemptive SJF\n");
    printf("2. Preemptive SJF (SRTF)\n");
    printf("3. Run Both\n");
    printf("Choose: ");

    if (scanf("%d", &choice) != 1 ||
        choice < 1 || choice > 3) {
        printf("Invalid choice.\n");
        return 1;
    }

    if (choice == 1 || choice == 3)
        sjf_non_preemptive(p, n);

    if (choice == 2 || choice == 3)
        sjf_preemptive(p, n);

    return 0;
}