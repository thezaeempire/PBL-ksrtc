# Deadlock Simulation System Using C and GTK 3

## 1. Abstract

The Deadlock Simulation System is a software application developed using the C programming language and GTK 3. It demonstrates operating system resource management, deadlock detection, deadlock avoidance, and recovery. The system models multiple processes competing for limited resources and provides a graphical interface to visualize system states and resource allocation.

## 2. Introduction

In an operating system, multiple processes may compete for limited resources. A deadlock occurs when processes are unable to proceed because the resources they need cannot be made available. This project simulates resource allocation and demonstrates methods used to prevent unsafe allocations, detect deadlocks, and recover from them.

## 3. Objectives

- Understand resource allocation in operating systems.
- Implement the Banker's algorithm for deadlock avoidance.
- Detect deadlocks using process requests and available resources.
- Simulate recovery by terminating a selected process and releasing its resources.
- Display system information through a GTK 3 graphical user interface.

## 4. Technologies Used

- **Programming language:** C
- **GUI framework:** GTK 3
- **Build tools:** GCC, Make, and pkg-config
- **Development environment:** Linux and Visual Studio Code

## 5. System Architecture

The system is divided into the following modules:

1. **Core module:** Maintains process information, resource totals, available resources, allocation matrices, maximum demand, and outstanding requests.
2. **Resource manager:** Validates resource requests and releases and coordinates safe allocation.
3. **Banker's algorithm:** Determines whether the system is in a safe state before granting a request.
4. **Deadlock detection:** Identifies processes that cannot complete under the current resource state.
5. **Recovery module:** Simulates process termination and releases its allocated resources.
6. **GUI module:** Displays the dashboard, resource matrices, and resource-allocation graph.

## 6. Algorithms

### 6.1 Banker's Algorithm

The Banker's algorithm calculates the remaining need of each process:

**Need = Maximum − Allocation**

It compares the remaining need with currently available resources. If processes can finish in a safe sequence, the system is considered safe. A resource request that makes the system unsafe is rejected.

### 6.2 Deadlock Detection

The detection algorithm starts with the available resource vector and checks whether each process's outstanding request can be satisfied. When a process can finish, its allocated resources are returned to the simulated available pool. Any process remaining unfinished is reported as potentially deadlocked under the detection model.

### 6.3 Deadlock Recovery

The recovery module simulates terminating a selected process. Its allocated resources are released, its allocation and outstanding request entries are cleared, and it is marked as terminated.

## 7. Data Structures

The system uses a shared `SystemState` structure containing:

- Number of processes and resource types
- Process and resource names
- Total and available resource vectors
- Allocation matrix
- Maximum-demand matrix
- Outstanding-request matrix
- Process termination status

## 8. Implementation

The application is implemented in modular C source and header files. Core resource management is separated from algorithm logic, while GTK 3 provides the graphical interface. The Makefile organizes compilation of the source modules.

## 9. Testing

The following tests should be performed and their actual results recorded:

| Test | Expected result |
|---|---|
| Check a safe state | A safe sequence is displayed |
| Submit a request exceeding available resources | The request is rejected |
| Submit a request exceeding the process's remaining need | The request is rejected |
| Submit a request that produces an unsafe state | The request is rolled back |
| Detect deadlock | Unfinished processes are reported when the detection conditions indicate deadlock |
| Recover a process | The selected process is terminated and its resources are released |

**Note:** Record the actual outcome of each test after running the application. Do not mark tests as passed unless verified.

## 10. Limitations

The application is a simulation and does not manage actual operating-system resources. Its accuracy depends on the correctness of the entered allocation, maximum-demand, and outstanding-request data. Interactive data-entry functionality depends on the GUI features implemented.

## 11. Future Enhancements

- Add forms for entering processes and resource types.
- Add interactive editing of allocation, maximum demand, and outstanding requests.
- Display safe sequences and deadlocked processes with clearer visual indicators.
- Add automated test cases and a test-results report.
- Improve graph visualization and provide step-by-step algorithm execution.

## 12. Conclusion

The Deadlock Simulation System demonstrates important operating system concepts through modular C implementation and a GTK 3 graphical interface. It brings together resource management, Banker's algorithm, deadlock detection, and recovery in an educational simulation.

## 13. References

1. Abraham Silberschatz, Peter B. Galvin, and Greg Gagne, *Operating System Concepts*.
2. Official GTK documentation: https://docs.gtk.org/gtk3/
3. GNU Compiler Collection documentation: https://gcc.gnu.org/
<img width="1563" height="1006" alt="Ubuntu Terminal Test Success" src="https://github.com/user-attachments/assets/bf691080-60cb-4ebd-ae7e-2e22c4a59d2f" />







