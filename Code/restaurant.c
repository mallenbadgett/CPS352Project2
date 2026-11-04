/*
 * CPS 356 - Operating Systems
 * Project 2 - Problem 2
 * Dining Philosophers Simulation
 *
 * Author: Camya Helton-Thomas, Jalen Anderson, Markala Allen-Badgett
 */


/*Section 1: 
*
*-Allows standard input/output with print commands
*-pthreads (threads, mutex, condition variables)
*/

#include <stdio.h>
#include <pthread.h>

/* Section 2
* -Max buffer capacity = 20
* -Max chefs = 5
* -Max waiters = 5 
*/

#define MAX_BUFFER 20
#define MAX_CHEFS 5
#define MAX_WAITERS 5

/* Section 3 (Global Variables)
  *  - Shared meal buffer
  *  - Where chefs add and where will waiters take
  *  - Number of meals in the buffer
  *  - Counters for the summary
  *  - Done flag so waiters know when to stop
  *  - Synchronization variables
*/

int buffer[MAX_BUFFER];
int capacity;
int size = 0;
int spot_added = 0;
int spot_stole = 0;
int chefs_per_meals;
int prepared = 0;
int served = 0;
int max_usage = 0;
int done = 0;
pthread_mutex_t lock;
pthread_cond_t not_full;
pthread_cond_t not_empty;

 
/*SECTION 4: BUFFER FUNCTIONS
  *  - put(): add a meal to the buffer
  *  - get(): take a meal out of the buffer
  *  - % capacity wraps the index back to 0
*/
 
void put(int meal)
{
    buffer[spot_added] = meal;
    spot_added = (spot_added + 1) % capacity;
    size++;
}
 
int get(void)
{
    int meal = buffer[spot_stole];
    spot_stole = (spot_stole + 1) % capacity;
    size--;
    return meal;
}
 
 
/*SECTION 5: CREATE CHEF FUNCTION
  *  - Get chef ID
  *  - Repeat M times
  *  - Lock the buffer
  *  - Wait while the buffer is full
  *  - Prepare a meal and put it in the buffer
  *  - Update max buffer usage
  *  - Signal a waiter and unlock
*/
 
void *chef(void *arg)
{
    int id = *(int *)arg;
 
    for (int i = 0; i < chefs_per_meals; i++)
    {
        pthread_mutex_lock(&lock);
 
        // Wait if the buffer is full
        while (size == capacity)
        {
            pthread_cond_wait(&not_full, &lock);
        }
 
        // Prepare a meal and add it to the buffer
        prepared++;
        put(prepared);
 
        // Track the largest buffer size
        if (size > max_usage)
        {
            max_usage = size;
        }
 
        printf("Chef %d prepared meal %d (buffer size = %d)\n", id, prepared, size);
 
        // Tell a waiter a meal is ready
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&lock);
    }
 
    return NULL;
}


 
/*SECTION 6:WAITER FUNCTION
  *  - Get waiter ID
  *  - Repeat until there are no meals left
  *  - Lock the buffer
  *  - Wait while the buffer is empty and chefs are still cooking
  *  - Stop if the buffer is empty and the chefs are done
  *  - Take a meal from the buffer and serve it
  *  - Signal a chef and unlock
*/
 
void *waiter(void *arg)
{
    int id = *(int *)arg;
 
    while (1)
    {
        pthread_mutex_lock(&lock);
 
        // Wait if the buffer is empty and chefs are still cooking
        while (size == 0 && done == 0)
        {
            pthread_cond_wait(&not_empty, &lock);
        }
 
        // Stop if the buffer is empty and all chefs are finished
        if (size == 0 && done == 1)
        {
            pthread_mutex_unlock(&lock);
            break;
        }
 
        // Take a meal and serve it
        int meal = get();
        served++;
 
        printf("Waiter %d served meal %d (buffer size = %d)\n", id, meal, size);
 
        // Tell a chef there is room
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&lock);
    }
 
    return NULL;
}
 
 
/*SECTION 7:MAIN FUNCTION
  *  - Ask user for C W B M
  *  - Validates input
  *  - Initialize synchronization variables
  *  - Create chef and waiter threads
  *  - Wait for chefs to finish, then set done
  *  - Wait for waiters to finish
  *  - Display execution summary
  *  - Destroy synchronization resources
*/
 
int main(void)
{
    int C, W;
 
    printf("C = number of chefs (1-5)\n");
    printf("W = number of waiters (1-5)\n");
    printf("B = buffer capacity (5-20)\n");
    printf("M = meals per chef (1-20)\n");
    printf("Enter C W B M: ");
 
    if (scanf("%d %d %d %d", &C, &W, &capacity, &chefs_per_meals) != 4 ||
        C < 1 || C > MAX_CHEFS || W < 1 || W > MAX_WAITERS 
        || capacity < 5 || capacity > MAX_BUFFER || chefs_per_meals < 1 || chefs_per_meals > 20)
    {
        printf("Invalid input\n");
        return 1;
    }
 
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);
 
    pthread_t chefs[MAX_CHEFS];
    pthread_t waiters[MAX_WAITERS];
    int chefIDs[MAX_CHEFS];
    int waiterIDs[MAX_WAITERS];
 
    for (int i = 0; i < C; i++) {
        chefIDs[i] = i + 1;
        pthread_create(&chefs[i], NULL, chef, &chefIDs[i]);
    }
 
    for (int i = 0; i < W; i++) {
        waiterIDs[i] = i + 1;
        pthread_create(&waiters[i], NULL, waiter, &waiterIDs[i]);
    }
 
    for (int i = 0; i < C; i++) {
        pthread_join(chefs[i], NULL);
    }
 
    // All chefs are finished: wake up every waiting waiter
    pthread_mutex_lock(&lock);
    done = 1;
    pthread_cond_broadcast(&not_empty);
    pthread_mutex_unlock(&lock);
 
    for (int i = 0; i < W; i++) {
        pthread_join(waiters[i], NULL);
    }
 
    printf("\nExecution Summary\n");
    printf("Total meals prepared by chefs: %d\n", prepared);
    printf("Total meals served by waiters: %d\n", served);
    printf("Maximum buffer: %d\n", max_usage);
 
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);
 
    return 0;
}
 
 
/*SECTION 6: CREATE WAITER FUNCTION
  *  - Get waiter ID
  *  - Repeat until there are no meals left
  *  - Lock the buffer
  *  - Wait while the buffer is empty and chefs are still cooking
  *  - Stop if the buffer is empty and the chefs are done
  *  - Take a meal from the buffer and serve it
  *  - Signal a chef and unlock
*/
 
void *waiter(void *arg)
{
    int id = *(int *)arg;
 
    while (1)
    {
        pthread_mutex_lock(&lock);
 
        // Will wait if chefs are still cooking
        while (size == 0 && done == 0)
        {
            pthread_cond_wait(&not_empty, &lock);
        }
 
        // Will stop if the buffer is empty and all chefs are finished
        if (size == 0 && done == 1)
        {
            pthread_mutex_unlock(&lock);
            break;
        }
 
        // Will take a meal and serve it
        int meal = get();
        served++;
        printf("Waiter %d served meal %d (buffer size = %d)\n", id, meal, size);
 
        // Tell a chef there is room to move forward
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&lock);
    }
 
    return NULL;
}
 
 
/*SECTION 7: MAIN FUNCTION
  *  - Ask user for C W B M inputs 
  *  - Validate input
  *  - Gets synchronization variables
  *  - Create chef and waiter threads
  *  - Wait for chefs to finish then set it as done
  *  - Wait for waiters to finish
  *  - Will display execution summary
  *  - Will destroy synchronization resources
*/
 
int main(void)
{
    int C, W;
    printf("C = number of chefs (1-5)\n");
    printf("W = number of waiters (1-5)\n");
    printf("B = buffer capacity (5-20)\n");
    printf("M = meals per chef (1-20)\n");
    printf("Enter C W B M: ");
 
    if (scanf("%d %d %d %d", &C, &W, &capacity, &chefs_per_meals) != 4 ||
        C < 1 || C > MAX_CHEFS ||
        W < 1 || W > MAX_WAITERS ||
        capacity < 5 || capacity > MAX_BUFFER ||
        chefs_per_meals < 1 || chefs_per_meals > 20)
    {
        printf("Invalid input, please try again\n");
        return 1;
    }
 
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);
 
    pthread_t chefs[MAX_CHEFS];
    pthread_t waiters[MAX_WAITERS];
    int chefIDs[MAX_CHEFS];
    int waiterIDs[MAX_WAITERS];
 
    for (int i = 0; i < C; i++) {
        chefIDs[i] = i + 1;
        pthread_create(&chefs[i], NULL, chef, &chefIDs[i]);
    }
 
    for (int i = 0; i < W; i++) {
        waiterIDs[i] = i + 1;
        pthread_create(&waiters[i], NULL, waiter, &waiterIDs[i]);
    }
 
    for (int i = 0; i < C; i++) {
        pthread_join(chefs[i], NULL);
    }
 
    // All chefs are finished by this point
    // this will wake up every remaining waiting waiter
    pthread_mutex_lock(&lock);
    done = 1;
    pthread_cond_broadcast(&not_empty);
    pthread_mutex_unlock(&lock);
 
    for (int i = 0; i < W; i++) {
        pthread_join(waiters[i], NULL);
    }
 
    printf("\nExecution Summary\n");
    printf("Total meals prepared: %d\n", prepared);
    printf("Total meals served: %d\n", served);
    printf("Maximum buffer usage: %d\n", max_usage);
 
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return 0;
}
 