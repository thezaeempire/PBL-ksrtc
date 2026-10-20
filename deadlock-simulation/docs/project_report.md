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

The core modules were compiled using GCC, and all five test programs executed successfully.

| Test | Actual result |
|---|---|
| System state initialization and validation | PASS |
| Resource request and release | PASS |
| Banker's algorithm and resource request safety | PASS |
| Deadlock detection | PASS |
| Process recovery and resource release | PASS |

**Core test summary:** 5 of 5 test programs passed.

**GUI build status:** Not yet verified. GTK 3 development files were unavailable in the development environment, so the graphical application has not yet been built or tested. Add screenshots after successfully launching the GUI.

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
