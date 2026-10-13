#include <stdio.h>
#include "scheduling.h"

int main(void)
{
    Process p[MAX];
    int n, choice, quantum;

    printf("=== CPU PROCESS SCHEDULING SIMULATOR ===\n");
    printf("Number of processes (1-%d): ", MAX);

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("\nProcess %d ID: ", i + 1);
        if (scanf("%d", &p[i].id) != 1)
            return 1;

        for (int j = 0; j < i; j++) {
            if (p[j].id == p[i].id) {
                printf("Process IDs must be unique.\n");
                return 1;
            }
        }

        printf("Arrival time: ");
        if (scanf("%d", &p[i].arrival) != 1 || p[i].arrival < 0)
            return 1;

        printf("Burst time: ");
        if (scanf("%d", &p[i].burst) != 1 || p[i].burst <= 0)
            return 1;

        printf("Priority (smaller number = higher priority): ");
        if (scanf("%d", &p[i].priority) != 1)
            return 1;

        p[i].remaining = p[i].burst;
        p[i].start = -1;
        p[i].completion = 0;
        p[i].waiting = 0;
        p[i].turnaround = 0;
        p[i].response = -1;
    }

    do {
        printf("\n===== MENU =====\n");
        printf("1. FCFS\n");
        printf("2. SJF Non-Preemptive\n");
        printf("3. SJF Preemptive (SRTF)\n");
        printf("4. Priority Non-Preemptive\n");
        printf("5. Priority Preemptive\n");
        printf("6. Round Robin\n");
        printf("0. Exit\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1)
            return 1;

        switch (choice) {
            case 1:
                fcfs_schedule(p, n);
                break;
            case 2:
                sjf_non_preemptive(p, n);
                break;
            case 3:
                sjf_preemptive(p, n);
                break;
            case 4:
                priority_non_preemptive(p, n);
                break;
            case 5:
                priority_preemptive(p, n);
                break;
            case 6:
                printf("Time quantum: ");
                if (scanf("%d", &quantum) != 1 || quantum <= 0) {
                    printf("Time quantum must be positive.\n");
                    return 1;
                }
                round_robin(p, n, quantum);
                break;
            case 0:
                printf("Exiting.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);

    return 0;
}
