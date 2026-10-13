#ifndef SCHEDULING_H
#define SCHEDULING_H

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

void fcfs_schedule(Process p[], int n);
void sjf_non_preemptive(Process p[], int n);
void sjf_preemptive(Process p[], int n);
void priority_non_preemptive(Process p[], int n);
void priority_preemptive(Process p[], int n);
void round_robin(Process p[], int n, int quantum);

void print_results(Process p[], int n, Segment g[], int segments);
void add_segment(Segment g[], int *count, int pid, int start, int end);

#endif
