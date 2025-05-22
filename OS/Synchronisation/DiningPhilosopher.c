#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int STATE[N];
sem_t mutex;
sem_t S[N];

void think(int i) {
    printf("Philosopher %d is thinking\n", i);
    usleep(1000000); // Simulate thinking
}

void eat(int i) {
    printf("Philosopher %d is eating\n", i);
    usleep(1000000); // Simulate eating
}

void test(int i) {
    if (STATE[i] == HUNGRY && STATE[LEFT] != EATING && STATE[RIGHT] != EATING) {
        STATE[i] = EATING;
        printf("Philosopher %d takes forks %d and %d\n", i, LEFT, RIGHT);
        printf("Philosopher %d starts eating\n", i);
        sem_post(&S[i]);
    }
}

void take_fork(int i) {
    sem_wait(&mutex);
    STATE[i] = HUNGRY;
    printf("Philosopher %d is hungry\n", i);
    test(i);
    sem_post(&mutex);
    sem_wait(&S[i]); // Wait if unable to eat
}

void put_fork(int i) {
    sem_wait(&mutex);
    printf("Philosopher %d puts down forks %d and %d\n", i, LEFT, RIGHT);
    printf("Philosopher %d is thinking again\n", i);
    STATE[i] = THINKING;
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);
}

void *philosopher(void *num) {
    int i = *(int *)num;
    while (1) {
        think(i);
        take_fork(i);
        eat(i);
        put_fork(i);
    }
}

int main() {
    pthread_t thread_id[N];
    int phil[N];
    sem_init(&mutex, 0, 1);
    
    for (int i = 0; i < N; i++) {
        sem_init(&S[i], 0, 0);
        STATE[i] = THINKING;
        phil[i] = i;
    }
    
    for (int i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
    }
    
    for (int i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }
    
    return 0;
}
