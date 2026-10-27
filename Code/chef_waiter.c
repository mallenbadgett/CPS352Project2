#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <semaphore.h>
#include <sched.h>

// Restaurant settings
int C;  // Number of chefs
int W;  // Number of waiters
int B;  // Buffer capacity
int M;  // Meals per chef

// Shared counters
int totalPrepared = 0;
int totalServed = 0;
int maxBufferUsage = 0;

// Shared meal buffer
int *buffer;
int bufferCount = 0;
int in = 0;
int out = 0;

// Synchronization tools
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

// Chef thread function
void *chef(void *arg) {
    int chefID = *(int *)arg;

    for (int i = 0; i < M; i++) {

        // Wait until there is space in the buffer
        sem_wait(&empty);

        // Lock the shared buffer
        pthread_mutex_lock(&mutex);

        // Prepare a new meal
        int mealID = ++totalPrepared;

        // Place the meal into the buffer
        buffer[in] = mealID;
        in = (in + 1) % B;
        bufferCount++;

        // Update maximum buffer usage
        if (bufferCount > maxBufferUsage) {
            maxBufferUsage = bufferCount;
        }

        printf("Chef %d prepared meal %d (buffer size = %d)\n",
               chefID, mealID, bufferCount);

        // Unlock the shared buffer
        pthread_mutex_unlock(&mutex);

        // Notify waiters that a meal is available
        sem_post(&full);
    }

    return NULL;
}
// Waiter thread function
void *waiter(void *arg) {
    int waiterID = *(int *)arg;

    while (1) {

        // Wait until a meal is available
        sem_wait(&full);

        // Lock the shared buffer
        pthread_mutex_lock(&mutex);

        // Check if all meals have been served
        if (totalServed >= C * M) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Remove a meal from the buffer
        int mealID = buffer[out];
        out = (out + 1) % B;
        bufferCount--;

        // Update the total meals served
        totalServed++;

        printf("Waiter %d served meal %d (buffer size = %d)\n",
               waiterID, mealID, bufferCount);

        // Unlock the shared buffer
        pthread_mutex_unlock(&mutex);

        // Notify chefs that a space is available
        sem_post(&empty);
    }

    return NULL;
}
// Main function
int main() {

    // Read restaurant settings
    printf("Enter number of chefs, waiters, buffer size, and meals: ");

    if (scanf("%d %d %d %d", &C, &W, &B, &M) != 4) {
        printf("Invalid input.\n");
        return 1;
    }

    // Validate input ranges
    if (C < 1 || C > 5 || W < 1 || W > 5 ||
        B < 5 || B > 20 || M < 1 || M > 20) {
        printf("Input values are out of range.\n");
        return 1;
    }

    // Allocate memory for the shared buffer
    buffer = malloc(B * sizeof(int));

    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Initialize synchronization tools
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, B);
    sem_init(&full, 0, 0);
      // Create arrays for chef and waiter threads
    pthread_t chefs[C];
    pthread_t waiters[W];

    // Store IDs for each chef and waiter
    int chefIDs[C];
    int waiterIDs[W];

    // Create chef threads
    for (int i = 0; i < C; i++) {
        chefIDs[i] = i + 1;

        pthread_create(&chefs[i], NULL, chef, &chefIDs[i]);
    }

    // Create waiter threads
    for (int i = 0; i < W; i++) {
        waiterIDs[i] = i + 1;

        pthread_create(&waiters[i], NULL, waiter, &waiterIDs[i]);
    }
        // Wait for all chefs to finish preparing meals
    for (int i = 0; i < C; i++) {
        pthread_join(chefs[i], NULL);
    }

    // Wait until all meals have been served
    while (1) {
        pthread_mutex_lock(&mutex);
        int finished = (totalServed == C * M);
        pthread_mutex_unlock(&mutex);

        if (finished) {
            break;
        }

        sched_yield();
    }

    // Wake up waiters so they can terminate
    for (int i = 0; i < W; i++) {
        sem_post(&full);
    }

    // Wait for all waiters to finish
    for (int i = 0; i < W; i++) {
        pthread_join(waiters[i], NULL);
    }

    // Print execution summary
    printf("\nExecution Summary\n");
    printf("Total meals prepared: %d\n", totalPrepared);
    printf("Total meals served: %d\n", totalServed);
    printf("Maximum buffer usage: %d\n", maxBufferUsage);

    // Clean up resources
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    free(buffer);

    return 0;
}
