#include <stdio.h>
#include <pthread.h>

int buffer[20]; //largest B is allowed the MAX
int capacity; 
int size = 0;
int spot_added = 0;
int spot_stole = 0;
int chefs_per_meals;
int total_num_meals;
int prepared = 0;
int served = 0;
int max_usage = 0;

 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
 
void *chef(void *arg) {

    int id = *(int *)arg;
 
    for (int i = 0; i < chefs_per_meals; i++) {

        pthread_mutex_lock(&lock);
 
        while (size == capacity) pthread_cond_wait(&not_full, &lock);
        prepared++;
        buffer[spot_added] = prepared;
        spot_added = (spot_added + 1) % capacity;
        size++;
 
        if (size > max_usage)
            max_usage = size;
 
        printf("Chef %d prepared meal %d (buffer size = %d)\n", id, prepared, size);
 
        pthread_cond_broadcast(&not_empty);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
 
void *waiter(void *arg) {
    int id = *(int *)arg;
 
    while (1) {
        pthread_mutex_lock(&lock);
 
        while (size == 0 && prepared < total_num_meals)
            pthread_cond_wait(&not_empty, &lock);
 
        if (size == 0) {
            pthread_mutex_unlock(&lock);
            break;
        }
 
        int meal = buffer[spot_stole];
        spot_stole = (spot_stole + 1) % capacity;
        size--;
        served++;
 
        printf("Waiter %d served meal %d (buffer size = %d)\n", id, meal, size);
 
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
 
int main() {
    int C, W;
    printf("C = number of chefs(1-5), W = number of waiters (1-5)\n");
    printf("B = buffer capacity (5-20), M = the number of meals each chef perpares(1-20)\n");
    printf("Enter C W B M: ");
    
    scanf("%d %d %d %d", &C, &W, &capacity, &chefs_per_meals);
    total_num_meals = C * chefs_per_meals;
 
    pthread_t chefs[5], waiters[5];
    int chef_ids[5], waiter_ids[5];
 
    for (int i = 0; i < C; i++) {
        chef_ids[i] = i + 1;
        pthread_create(&chefs[i], NULL, chef, &chef_ids[i]);
    }
    for (int i = 0; i < W; i++) {
        waiter_ids[i] = i + 1;
        pthread_create(&waiters[i], NULL, waiter, &waiter_ids[i]);
    }
 
    for (int i = 0; i < C; i++)
        pthread_join(chefs[i], NULL);

    for (int i = 0; i < W; i++)
        pthread_join(waiters[i], NULL);
 
    printf("\nExecution Summary\n");
    printf("Total meals prepared: %d\n", prepared);
    printf("Total meals served: %d\n", served);
    printf("Maximum buffer usage: %d\n", max_usage);
    return 0;
}
