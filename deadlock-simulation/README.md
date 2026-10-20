# Deadlock Simulation System

## Project Overview

The Deadlock Simulation System is a C-based desktop
application developed using GTK 3.

It demonstrates operating system resource allocation,
deadlock avoidance, deadlock detection, and recovery.

## Technologies Used

- C programming language
- GTK 3 for the graphical user interface
- GCC compiler
- Make
- Git and GitHub for version control

## Project Structure

deadlock-simulation/
├── main.c
├── system_state.h
├── system_state.c
├── Makefile
├── README.md
├── gui/
├── algorithms/
├── core/
├── tests/
└── docs/

## Current Features

- GTK desktop dashboard
- Process and resource information
- Allocation matrix display
- Maximum matrix display
- Need matrix calculation
- Available resource vector
- Input validation
- Buttons for Banker's Algorithm,
  deadlock detection, and recovery

## Setup Requirements

Ubuntu or another compatible Linux environment
with GCC, Make, pkg-config, and GTK 3 installed.

Install the dependencies on Ubuntu:

    sudo apt update
    sudo apt install build-essential pkg-config libgtk-3-dev

## Build Instructions

Open a terminal in the project directory.

Compile the project:

    make

Run the application:

    make run

Remove the compiled executable:

    make clean

## Team Responsibilities

- Member 1: GUI and project integration
- Member 2: Banker's Algorithm
- Member 3: Resource Allocation Graph
- Member 4: Deadlock detection and recovery
- Member 5: Testing and documentation

## Development Status

The GUI and shared system-state module provide
the initial project foundation.

The algorithm buttons are integration placeholders.
The corresponding algorithms must be implemented,
tested, and connected before final submission.

## Academic Project

Operating Systems — Week 4

Design and Implementation of an Interactive
Deadlock Simulation System