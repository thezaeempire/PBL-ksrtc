# CPU Process Scheduling Simulator

## Project Description
This project implements a CPU Process Scheduling Simulator in C.
It allows users to select different scheduling algorithms and view
their Gantt charts and performance metrics.

## Scheduling Algorithms
1. First Come, First Served (FCFS)
2. Shortest Job First (SJF) - Non-Preemptive
3. Shortest Remaining Time First (SRTF) - Preemptive SJF
4. Priority Scheduling - Non-Preemptive
5. Priority Scheduling - Preemptive
6. Round Robin (RR)

## Features
- Interactive menu to select a scheduling algorithm
- Gantt chart generation
- Completion Time (CT)
- Turnaround Time (TAT)
- Waiting Time (WT)
- Response Time (RT)
- Average Waiting Time
- Average Turnaround Time
- CPU Utilization

## Requirements
- GCC compiler
- Linux terminal or another environment that supports GCC

## Project Files
- `main.c` - Main program and menu
- `scheduling.h` - Shared structures and function declarations
- `fcfs.c` - FCFS scheduling
- `sjf.c` - SJF and SRTF scheduling
- `priority.c` - Priority scheduling algorithms
- `round_robin.c` - Round Robin scheduling
- `metrics.c` - Scheduling metrics and Gantt chart output

## Compilation
Run the following command from this directory:

```bash
gcc -std=c11 -Wall -Wextra main.c fcfs.c sjf.c priority.c round_robin.c metrics.c -o scheduler
```

## Execution
Run the compiled program:

```bash
./scheduler
```

Enter the number of processes and the requested process details.
Choose an algorithm from the menu. For Round Robin, also enter the
time quantum when prompted.

## Testing
The six scheduling algorithms were run with sample process inputs.
Further testing with different arrival times, burst times, priorities,
and time quantum values is recommended.

