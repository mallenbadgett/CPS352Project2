CPS 356 – Operating Systems
Project 2 – Fall 2026

Team Members:Camya Helton-Thomas, ,

Instructor: Dr. Tam Nguyen
Programming Language: C
Operating System: Linux / Ubuntu
Compiler: GCC

1. PROJECT OVERVIEW

This project focuses on multithreading, synchronization, and shared resources in operating systems. The project contains two programs written in C using POSIX threads.

Problem 1: Chef-Waiter Restaurant System

The first program simulates a restaurant where multiple chefs prepare meals and multiple waiters serve them. The chefs and waiters work at the same time and share a limited buffer that stores prepared meals.

The program uses mutex locks and semaphores to control access to the shared buffer and prevent race conditions, buffer overflow, and buffer underflow.

Problem 2: Dining Philosophers Simulation

The second program simulates five philosophers sitting around a table with five forks. Each philosopher switches between thinking, hungry, eating, and done states.

The program uses mutex locks, condition variables, and a ticket system to manage fork access and prevent deadlock and starvation.

2. SYSTEM REQUIREMENTS

The following tools are needed to compile and run the programs:

- Linux or Ubuntu operating system
- GCC compiler
- POSIX Threads library
- Terminal

3. PROBLEM 1 – COMPILATION INSTRUCTIONS

Open the terminal and navigate to the folder containing restaurant.c.

Compile the program using the following command:

gcc -pthread restaurant.c -o restaurant

This command compiles the source code and creates an executable named restaurant.

4. PROBLEM 1 – EXECUTION INSTRUCTIONS

Run the program using:

./restaurant

The program asks the user to enter four integers:

C W B M

C represents the number of chefs (1–5).

W represents the number of waiters (1–5).

B represents the buffer capacity (5–20).

M represents the number of meals each chef prepares (1–20).

Example input:

2 2 5 3

This means there are two chefs, two waiters, a buffer capacity of five meals, and three meals prepared by each chef.

The total number of meals is calculated by multiplying the number of chefs by the number of meals per chef.

2 × 3 = 6 meals

5. PROBLEM 1 – EXPECTED OUTPUT

The program displays messages showing when chefs prepare meals and when waiters serve meals.

Example output:

Chef 1 prepared meal 1 (buffer size = 1)
Chef 2 prepared meal 2 (buffer size = 2)
Waiter 1 served meal 1 (buffer size = 1)
Waiter 2 served meal 2 (buffer size = 0)

The program continues until all meals have been prepared and served.

At the end, the program displays an execution summary.

Example:

Execution Summary
Total meals prepared: 6
Total meals served: 6
Maximum buffer usage: Varies

The order of the messages and maximum buffer usage may change between executions because the threads run concurrently.

6. PROBLEM 1 – SYNCHRONIZATION

The restaurant program uses several synchronization methods to manage shared resources.

Mutex Lock:

The mutex prevents multiple threads from modifying the shared buffer and counters at the same time.

Empty Semaphore:

The empty semaphore tracks available spaces in the buffer. If the buffer is full, chefs must wait until a waiter removes a meal.

Full Semaphore:

The full semaphore tracks the number of meals available. If the buffer is empty, waiters must wait until a chef prepares another meal.

Circular Buffer:

The program uses a circular buffer to store meals. The insertion and removal positions are updated using modulo arithmetic.

Thread Termination:

After all meals are served, the main thread signals the waiters so they can finish execution.

7. PROBLEM 2 – COMPILATION INSTRUCTIONS

Open the terminal and navigate to the folder containing dining_philosophers.c.

Compile the program using:

gcc -pthread dining_philosophers.c -o dining_philosophers

This creates an executable named dining_philosophers.

8. PROBLEM 2 – EXECUTION INSTRUCTIONS

Run the program using:

./dining_philosophers

The program asks the user to enter the number of eating cycles each philosopher must complete.

Example input:

2

This means each philosopher must complete two eating cycles.

The program creates five philosopher threads. Each philosopher changes between thinking, hungry, eating, and done states.

9. PROBLEM 2 – EXPECTED OUTPUT

The program displays information about the current state of each philosopher, the number of completed eating cycles, and the status of each fork.

Example output:

Dining Philosophers

Philosopher 0: Thinking
Philosopher 1: Hungry
Philosopher 2: Eating
Philosopher 3: Thinking
Philosopher 4: Hungry

Eating Cycles

Philosopher 0: 0/2 cycles
Philosopher 1: 0/2 cycles
Philosopher 2: 0/2 cycles
Philosopher 3: 0/2 cycles
Philosopher 4: 0/2 cycles

Fork Status

Fork 0: Available
Fork 1: Available
Fork 2: In use
Fork 3: In use
Fork 4: Available

The displayed information changes as the philosophers continue through their eating cycles.

Once the simulation finishes, all five philosophers should display the Done state and have completed the required number of eating cycles.

All forks should also be available.

10. PROBLEM 2 – SYNCHRONIZATION

The Dining Philosophers program uses synchronization to control access to the five shared forks.

Mutex Lock:

The mutex protects shared variables, including philosopher states, fork availability, and ticket counters.

Condition Variable:

The condition variable allows philosophers to wait until their ticket is being served and both required forks are available.

Ticket System:

Each philosopher receives a ticket when becoming hungry. The ticket system controls the order in which philosophers are allowed to eat.

Fork Tracking:

The program uses an array to track whether each fork is available or currently being used.

Deadlock Prevention:

The program coordinates access to both forks through a shared mutex. This prevents philosophers from holding one fork while waiting for another.

Starvation Prevention:

The ticket system gives waiting philosophers turns based on the order in which they requested access.

11. PROJECT FILE STRUCTURE

Project2/

Code/
restaurant.c
dining_philosophers.c

Report/
Report.pdf

Presentation/
Presentation.pptx

README.txt

The Code folder contains the source code for both problems. The Report folder contains the technical report explaining the solutions and results. The Presentation folder contains the project slides. The README.txt file provides instructions for compiling and running both programs. There is also some test files.

12. ADDITIONAL NOTES

Both programs were developed in C using POSIX threads.

The execution order may vary because multiple threads operate concurrently.

Problem 1 checks that the input values are within the required ranges before starting execution.

Problem 1 uses dynamically allocated memory for the shared buffer and releases that memory after execution.

Problem 2 requires the user to enter a positive number of eating cycles.

Problem 2 uses a ticket-based system to coordinate philosopher access to shared forks.

Both programs demonstrate how synchronization can be used to manage shared resources in an operating system.
