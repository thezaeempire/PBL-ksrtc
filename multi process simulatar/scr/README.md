# Multi-Process Simulator

## Overview

A C-based multi-process simulator demonstrating communication between
independent processes using IPC mechanisms.

## Architecture

The simulator consists of three main processes:

- **UI Process** - Accepts commands from the user.
- **Core Process** - Processes commands and performs CPU, Stack and Queue operations.
- **Logger Process** - Records simulator events and operations in `simulator.log`.

### Process Flow

UI Process → Core Process → Logger Process

The architecture diagram is available in:

`IPC ARCHITECTURE DIAGRAM.jpeg`

## Supported Commands

```text
ADD 10 20
SUB 20 5
PUSH 10
POP
ENQUEUE 10
DEQUEUE

