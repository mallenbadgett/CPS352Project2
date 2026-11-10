
/*
 * CPS 356 - Operating Systems
 * Project 2 - Problem 2
 * Dining Philosophers Simulation
 *
 * Authors: Camya Helton-Thomas, Jalen Anderson, Markala
 *
 * Synchronization:
 * Mutex locks, condition variable, and ticket-based waiter
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/* SECTION 1: DEFINE CONSTANTS */

#define NUM_PHILOSOPHERS 7

#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define DONE 3

/* SECTION 2: GLOBAL VARIABLES */

pthread_mutex_t waiterMutex;
pthread_cond_t waiterCond;

int states[NUM_PHILOSOPHERS];
int forkInUse[NUM_PHILOSOPHERS];
int completedCycles[NUM_PHILOSOPHERS];

int T;
int nextTicket = 0;
int servingTicket = 0;

/* SECTION 3: GET PHILOSOPHER STATE */

const char *getStateName(int state)
{
    switch (state)
    {
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

/* SECTION 4: DISPLAY FUNCTION */

void display(void)
{
    pthread_mutex_lock(&waiterMutex);

    printf("\n========== DINING PHILOSOPHERS ==========\n");

    printf("\nPhilosopher States:\n");

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        printf("Philosopher %d: %s\n",
               i, getStateName(states[i]));
    }

    printf("\nEating Cycles:\n");

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        printf("Philosopher %d: %d/%d cycles\n",
               i, completedCycles[i], T);
    }

    printf("\nFork Status:\n");

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        if (forkInUse[i] == 0)
        {
            printf("Fork %d: Available\n", i);
        }
        else
        {
            printf("Fork %d: In use\n", i);
        }
    }

    printf("=========================================\n");

    pthread_mutex_unlock(&waiterMutex);
}

/* SECTION 5: PHILOSOPHER FUNCTION */

void *philosopher(void *arg)
{
    int id = *(int *)arg;

    int leftFork = id;
    int rightFork = (id + 1) % NUM_PHILOSOPHERS;

    while (1)
    {
        /* Check completed eating cycles */

        pthread_mutex_lock(&waiterMutex);

        if (completedCycles[id] >= T)
        {
            pthread_mutex_unlock(&waiterMutex);
            break;
        }

        /* Philosopher starts thinking */

        states[id] = THINKING;

        pthread_mutex_unlock(&waiterMutex);

        display();

        sleep(1);

        /* Philosopher becomes hungry */

        pthread_mutex_lock(&waiterMutex);

        states[id] = HUNGRY;

        /* Assign a ticket */

        int ticket = nextTicket++;

        pthread_mutex_unlock(&waiterMutex);

        display();

        /* Wait for turn and available forks */

        pthread_mutex_lock(&waiterMutex);

        while (ticket != servingTicket ||
               forkInUse[leftFork] == 1 ||
               forkInUse[rightFork] == 1)
        {
            pthread_cond_wait(&waiterCond, &waiterMutex);
        }

        /* Pick up both forks */

        forkInUse[leftFork] = 1;
        forkInUse[rightFork] = 1;

        /* Advance ticket after acquiring forks */

        servingTicket++;

        /* Philosopher starts eating */

        states[id] = EATING;

        /* Wake up the next waiting philosopher */

        pthread_cond_broadcast(&waiterCond);

        pthread_mutex_unlock(&waiterMutex);

        display();

        sleep(1);

        /* Philosopher finishes eating */

        pthread_mutex_lock(&waiterMutex);

        /* Release both forks */

        forkInUse[leftFork] = 0;
        forkInUse[rightFork] = 0;

        /* Update completed cycles */

        completedCycles[id]++;

        /* Update philosopher state */

        if (completedCycles[id] >= T)
        {
            states[id] = DONE;
        }
        else
        {
            states[id] = THINKING;
        }

        /* Notify waiting philosophers */

        pthread_cond_broadcast(&waiterCond);

        pthread_mutex_unlock(&waiterMutex);

        display();
    }

    return NULL;
}

/* SECTION 6: MAIN FUNCTION */

int main(void)
{
    printf("Enter the number of eating cycles per philosopher: ");

    if (scanf("%d", &T) != 1 || T < 1)
    {
        printf("Invalid number of eating cycles.\n");
        return 1;
    }

    /* Initialize synchronization variables */

    pthread_mutex_init(&waiterMutex, NULL);
    pthread_cond_init(&waiterCond, NULL);

    /* Initialize philosopher states and forks */

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        states[i] = THINKING;
        forkInUse[i] = 0;
        completedCycles[i] = 0;
    }

    nextTicket = 0;
    servingTicket = 0;

    /* Create philosopher threads */

    pthread_t threads[NUM_PHILOSOPHERS];
    int philosopherIDs[NUM_PHILOSOPHERS];

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        philosopherIDs[i] = i;

        pthread_create(&threads[i], NULL,
                       philosopher, &philosopherIDs[i]);
    }

    /* Wait for all philosophers to finish */

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    /* Display final results */

    printf("\n========== FINAL RESULTS ==========\n");

    display();

    int totalMeals = 0;

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        totalMeals += completedCycles[i];
    }

    printf("\nTotal meals completed: %d\n", totalMeals);
    printf("Expected total meals: %d\n",
           NUM_PHILOSOPHERS * T);

    if (totalMeals == NUM_PHILOSOPHERS * T)
    {
        printf("SUCCESS: All philosophers completed their meals.\n");
    }
    else
    {
        printf("ERROR: Some meals were not completed.\n");
    }

    /* Destroy synchronization resources */

    pthread_cond_destroy(&waiterCond);
    pthread_mutex_destroy(&waiterMutex);

    printf("\nSimulation finished.\n");

    return 0;
}
