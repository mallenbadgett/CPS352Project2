# CPS 356 – Operating Systems
## Project 2 – Fall 2026

**Instructor:** Dr. Tam Nguyen  
**Team Members:** Camya Helton-Thomas, Jalen Anderson, Markala  
**Programming Language:** C  
**Operating System:** Linux / Ubuntu  
**Compiler:** GCC

---

## 1. Project Overview

This project demonstrates multithreading, synchronization, and shared resource management through two simulations.

**Problem 1: Chef-Waiter Restaurant System**

The first program simulates a restaurant where multiple chefs prepare meals and multiple waiters serve them. Chefs and waiters execute concurrently and communicate through a shared bounded buffer.

The program uses POSIX threads, mutex locks, and semaphores to prevent race conditions, buffer overflow, and buffer underflow.

**Problem 2: Dining Philosophers Simulation**

The second program simulates five philosophers sitting around a table with five shared forks. Each philosopher alternates between thinking, becoming hungry, and eating.

The program uses a ticket-based synchronization system, a mutex, and a condition variable to coordinate fork access and prevent deadlock and starvation.

## 2. System Requirements

- Linux or Ubuntu operating system
- GCC compiler
- POSIX Threads (pthreads) library
- Terminal or command-line interface

## 3. Problem 1 – Compilation Instructions

Open a terminal and navigate to the folder containing the source code.

Compile using:

`gcc -pthread chef_waiter.c -o chef_waiter`

Replace `chef_waiter.c` with the actual filename if different.

## 4. Problem 1 – Execution Instructions

Run the program:

`./chef_waiter`

The program asks the user to enter four integers:

`C W B M`

Where:

- C = Number of chefs (1–5)
- W = Number of waiters (1–5)
- B = Buffer capacity (5–20)
- M = Number of meals each chef prepares (1–20)

Example input:

`2 2 5 3`

This creates two chef threads, two waiter threads, a buffer capacity of five, and three meals per chef.

The total number of meals is:

2 × 3 = 6 meals.

## 5. Problem 1 – Expected Output

The program prints messages when chefs prepare meals and waiters serve them.

Example:

    Chef 1 prepared meal 1 (buffer size = 1)
    Chef 2 prepared meal 2 (buffer size = 2)
    Waiter 1 served meal 1 (buffer size = 1)
    Waiter 2 served meal 2 (buffer size = 0)

The program continues until all meals are prepared and served.

Expected execution summary:

    Execution Summary
    Total meals prepared: 6
    Total meals served: 6
    Maximum buffer usage: [varies]

The execution order and maximum buffer usage depend on thread scheduling.

## 6. Problem 1 – Synchronization

**Mutex Lock:** Protects the shared meal buffer and counters from simultaneous modification.

**Empty Semaphore:** Tracks available buffer spaces and prevents chefs from adding meals when the buffer is full.

**Full Semaphore:** Tracks available meals and prevents waiters from removing meals when the buffer is empty.

**Circular Buffer:** Uses modulo arithmetic to manage meal insertion and removal positions.

**Thread Termination:** Once all meals are served, the main thread wakes waiting waiter threads so they can terminate.

## 7. Problem 2 – Compilation Instructions

Navigate to the folder containing the Dining Philosophers source code.

Compile using:

`gcc -pthread dining_philosophers.c -o dining_philosophers`

Replace `dining_philosophers.c` with the actual filename if different.

## 8. Problem 2 – Execution Instructions

Run the program:

`./dining_philosophers`

The program prompts:

`Enter the number of eating cycles per philosopher:`

Example input:

`2`

This means each of the five philosophers must complete two eating cycles.

The program creates five philosopher threads. Each philosopher alternates between thinking, hungry, eating, and done states.

## 9. Problem 2 – Expected Output

The program displays each philosopher's current state, completed eating cycles, and the availability of each fork.

Example:

    Dining Philosophers

    Philosopher 0: Thinking
    Philosopher 1: Hungry
    Philosopher 2: Eating
    Philosopher 3: Thinking
    Philosopher 4: Hungry

    Eating Cycles

    Philosopher 0: 0/2 cycles
    Philosopher 1: 0/2 cycles
    Philosopher 2: 1/2 cycles
    Philosopher 3: 0/2 cycles
    Philosopher 4: 0/2 cycles

    Fork Status

    Fork 0: Available
    Fork 1: Available
    Fork 2: In use
    Fork 3: In use
    Fork 4: Available

The displayed states change throughout execution.

At completion, all five philosophers should show:

`2/2 cycles`

and all philosophers should have the state:

`Done`

All forks should be available.

## 10. Problem 2 – Synchronization

**Mutex Lock:** Protects shared philosopher states, fork availability, and ticket counters.

**Condition Variable:** Allows philosophers to wait until their turn arrives and the required forks are available.

**Ticket System:** Each hungry philosopher receives a ticket. Philosophers eat in ticket order, providing fair access to the shared resources.

**Fork Tracking:** The program tracks whether each fork is available or in use.

**Deadlock Prevention:** Philosophers coordinate fork acquisition through a shared synchronization mechanism rather than holding one fork while waiting for another.

**Starvation Prevention:** The ticket-based ordering ensures that waiting philosophers receive turns in the order their tickets were assigned.

## 11. Project File Structure

    Project2/
    |
    |-- Code/
    |   |-- chef_waiter.c
    |   |-- dining_philosophers.c
    |
    |-- Report/
    |   |-- Report.pdf
    |
    |-- Presentation/
    |   |-- Presentation.pptx
    |
    |-- README.txt

The filenames should match the actual submitted files.

## 12. Additional Notes

- Both programs use POSIX threads for concurrent execution.
- Thread execution order may vary between runs.
- Problem 1 validates the input ranges before creating threads.
- Problem 1 dynamically allocates and releases its shared meal buffer.
- Problem 2 requires a positive number of eating cycles.
- Problem 2 uses a ticket-based system to coordinate philosopher access to forks.
- Both programs use synchronization mechanisms to protect shared resources.
- Compile and test both programs before submission.


