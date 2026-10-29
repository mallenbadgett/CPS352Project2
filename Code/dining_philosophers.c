/*
 * CPS 356 - Operating Systems
 * Project 2 - Problem 2
 * Dining Philosophers Simulation
 *
 * Author: Camya Helton-Thomas, Jalen Anderson, Markala
 */


SECTION 1: INCLUDE LIBRARIES
    - Standard input/output
    - pthreads
    - Semaphores or mutexes
    - Time/delay functions

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


SECTION 2: DEFINE CONSTANTS
    - Number of philosophers = 5
    - Number of forks = 5

int philosophers= 5;

int forks = 5;

SECTION 3: DECLARE GLOBAL VARIABLES
    - Array for five forks
    - Array for philosopher states
    - Array for completed eating cycles
    - Number of eating cycles (T)
    - Synchronization variables


int[] forkArray = new int[5];

string[] states = ["Thinking", "Becoming hungry", "Eating"];


SECTION 4: CREATE DISPLAY FUNCTION
    - Show five philosophers around a table
    - Display each philosopher's current state
    - Display the status of each fork
    - Show completed eating cycles

SECTION 5: CREATE PHILOSOPHER FUNCTION
    - Get philosopher ID
    - Repeat until T eating cycles are completed
    - Change state to THINKING
    - Change state to HUNGRY
    - Wait until both forks can be acquired safely
    - Pick up forks
    - Change state to EATING
    - Eat
    - Release both forks
    - Update completed cycles
    - Change state to THINKING or DONE

SECTION 6: CREATE MAIN FUNCTION
    - Ask user for T
    - Validate input
    - Initialize five forks
    - Initialize philosopher states
    - Create five philosopher threads
    - Wait for all threads to finish
    - Display final results
    - Destroy synchronization resources
    - End program
