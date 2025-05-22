#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

sem_t s, empty, full;
int queue[5], avail = 0;

void *producer(void *);
void *consumer(void *);

int main() {
    pthread_t prod_h, cons_h;

    sem_init(&s, 0, 1);
    sem_init(&empty, 0, 5);  // Buffer size is 5
    sem_init(&full, 0, 0);

    pthread_create(&prod_h, NULL, producer, NULL);
    pthread_create(&cons_h, NULL, consumer, NULL);

    pthread_join(prod_h, NULL);
    pthread_join(cons_h, NULL);

    return 0;
}

void *producer(void *args) {
    int item;
    
    while (1) { // Infinite loop
        item = rand() % 1000;

        sem_wait(&empty); // Wait for an empty slot
        sem_wait(&s);     // Lock the queue

        queue[avail] = item;
        avail++;
        printf("Produced: %d (Buffer count: %d)\n", item, avail);

        sem_post(&s);    // Unlock the queue
        sem_post(&full); // Signal that an item is available

        sleep(1);
    }
}

void *consumer(void *args) {
    int my_item;

    while (1) { // Infinite loop
        sem_wait(&full); // Wait for an item to be available
        sem_wait(&s);    // Lock the queue

        avail--;
        my_item = queue[avail];

        printf("Consumed: %d (Buffer count: %d)\n", my_item, avail);

        sem_post(&s);    // Unlock the queue
        sem_post(&empty); // Signal that a slot is free

        sleep(1);
    }
}
