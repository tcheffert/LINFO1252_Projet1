#include "../headers/semaphore_interface.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ITEMS 10 // Number of items to produce/consume

// Shared semaphore and variables
semaphore_t semaphore;
int shared_resource = 0; // A simple shared resource to modify

void* producer(void* arg) {
    for (int i = 0; i < ITEMS; i++) {
        semaphore_signal(&semaphore); // Increment semaphore to indicate resource is available
        printf("Producer: Produced item %d\n", i + 1);
        usleep(100000); // Simulate some work
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < ITEMS; i++) {
        semaphore_wait(&semaphore); // Wait for resource to be available
        printf("Consumer: Consumed item %d\n", i + 1);
        usleep(150000); // Simulate some work
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Initialize the semaphore with an initial value of 0
    semaphore_init(&semaphore, 0);

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for both threads to complete
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Clean up the semaphore
    semaphore_destroy(&semaphore);

    printf("Test complete.\n");
    return 0;
}
