/*
 * CPS 356 - Operating Systems
 * Project 2 - Problem 2
 * Dining Philosophers Simulation
 *
 * Author: Camya Helton-Thomas, Jalen Anderson, Markala
 */


/*SECTION 1: INCLUDE LIBRARIES
  *  - Standard input/output
   * - pthreads
  *  - Semaphores or mutexes
  *  - Time/delay functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <unistd.h>


/*SECTION 2: DEFINE CONSTANTS
  *  - Number of philosophers = 5
  *  - Number of forks = 5
*/

int philosophers= 5;

int forks = 5;

/*SECTION 3: DECLARE GLOBAL VARIABLES
  *  - Array for five forks
   * - Array for philosopher states
   * - Array for completed eating cycles
   * - Number of eating cycles (T)
    *- Synchronization variables
*/



#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define DONE 3

pthread_mutex_t forkArray[5];

pthread_mutex_t displayMutex;

pthread_mutex_t waiterMutex;

pthread_cond_t waiterCond;
int states[5];
int forkInUse[5];
int completed[5];

int completedCycles[5];
int T;
int nextTicket = 0;
int servingTicket = 0;

/*SECTION 4: CREATE DISPLAY FUNCTION
 *   - Show five philosophers around a table
 *   - Display each philosopher's current state
 *   - Display the status of each fork
 *   - Show completed eating cycles
 */

const char *getStateName(int state) {
    switch (state) {
        case THINKING:
            return "Thinking";

        case HUNGRY:
            return "Hungry";

        case EATING:
            return "Eating";

        case DONE:
            return "Done";

        default:
            return "Unknown";
    }
}


void display(void)
{
    pthread_mutex_lock(&waiterMutex);

    printf("\n Dining Philosophers\n");

    for (int i = 0; i < 5; i++) {
        printf("Philosopher %d: %s\n",
               i, getStateName(states[i]));
    }

    printf("\n Eating Cycles\n");

    for (int i = 0; i < 5; i++) {
        printf("Philosopher %d: %d/%d cycles\n",
               i, completedCycles[i], T);
    }

    printf("\n Fork Status\n");

    for (int i = 0; i < 5; i++) {
        if (forkInUse[i] == 0) {
            printf("Fork %d: Available\n", i);
        } else {
            printf("Fork %d: In use\n", i);
        }
    }

    pthread_mutex_unlock(&waiterMutex);
}


/*SECTION 5: CREATE PHILOSOPHER FUNCTION
  *  - Get philosopher ID
  *  - Repeat until T eating cycles are completed
  *  - Change state to THINKING
  *  - Change state to HUNGRY
  *  - Wait until both forks can be acquired safely
  *  - Pick up forks
  *  - Change state to EATING
  *  - Eat
  *  - Release both forks
  *  - Update completed cycles
  *  - Change state to THINKING or DONE
 */


void *philosopher(void *arg)
{
    int id = *(int *)arg;

    int leftFork = id;
    int rightFork = (id + 1) % 5;

    while (1)
    {
        // Check if this philosopher has completed all cycles
        pthread_mutex_lock(&waiterMutex);

        if (completedCycles[id] >= T)
        {
            pthread_mutex_unlock(&waiterMutex);
            break;
        }

        // Think
        states[id] = THINKING;
        pthread_mutex_unlock(&waiterMutex);

        display();

        sleep(1);

        // Become hungry and receive a ticket
        pthread_mutex_lock(&waiterMutex);

        states[id] = HUNGRY;
        int ticket = nextTicket++;

        pthread_mutex_unlock(&waiterMutex);

        display();

        // Wait for our turn and for both forks to be available
        pthread_mutex_lock(&waiterMutex);

        while (ticket != servingTicket ||
               forkInUse[leftFork] == 1 ||
               forkInUse[rightFork] == 1)
        {
            pthread_cond_wait(&waiterCond, &waiterMutex);
        }

        // Pick up both forks
        forkInUse[leftFork] = 1;
        forkInUse[rightFork] = 1;

        // Eat
        states[id] = EATING;

        pthread_mutex_unlock(&waiterMutex);

        display();

        sleep(1);

        // Put down both forks and finish one cycle
        pthread_mutex_lock(&waiterMutex);

        forkInUse[leftFork] = 0;
        forkInUse[rightFork] = 0;

        completedCycles[id]++;

        // Advance to the next philosopher's ticket
        servingTicket++;

        // Update the philosopher's state
        if (completedCycles[id] >= T)
        {
            states[id] = DONE;
        }
        else
        {
            states[id] = THINKING;
        }

        // Wake up waiting philosophers
        pthread_cond_broadcast(&waiterCond);

        pthread_mutex_unlock(&waiterMutex);

        display();
    }

    return NULL;
}



/*SECTION 6: CREATE MAIN FUNCTION
*  - Ask user for T
*    - Validate input
*    - Initialize five forks
*    - Initialize philosopher states
*    - Create five philosopher threads
*    - Wait for all threads to finish
*    - Display final results
*    - Destroy synchronization resources
*    - End program
*/

int main(void)
{
    printf("Enter the number of eating cycles per philosopher: ");

    if (scanf("%d", &T) != 1 || T < 1) {
        printf("Invalid number of eating cycles per philosopher\n");
        return 1;
    }

    pthread_mutex_init(&waiterMutex, NULL);
    pthread_mutex_init(&displayMutex, NULL);
    pthread_cond_init(&waiterCond, NULL);

    for (int i = 0; i < 5; i++) {
        pthread_mutex_init(&forkArray[i], NULL);

        states[i] = THINKING;
        completedCycles[i] = 0;
        forkInUse[i] = 0;
    }

    nextTicket = 0;
    servingTicket = 0;

    pthread_t threads[5];
    int philosopherIDs[5];

    for (int i = 0; i < 5; i++) {
        philosopherIDs[i] = i;

        pthread_create(&threads[i], NULL,
                       philosopher, &philosopherIDs[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    display();

    pthread_mutex_destroy(&waiterCond);
    pthread_mutex_destroy(&waiterMutex);
    pthread_cond_destroy(&displayMutex);

    return 0;
}
