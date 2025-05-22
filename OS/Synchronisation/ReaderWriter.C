#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

sem_t s, db;
int rc = 0; // read count

void readDatabase(int n) {
    printf("Reader %d is Reading Database\n",n);
}

void writeDatabase() {
    printf("Writing to Database\n");
}

void useDatabase(int n) {
    printf("Reader %d is Using Database\n",n);
}

void thinkUpdata() {
    printf("Thinking data to write\n");
}

void *reader(void *arg) {
    int reader = *(int *)arg;
    while (1) {
        // Enter critical section to update rc
        sem_wait(&s);
        
        // If this is the first reader, lock the database
        rc++;
        if (rc == 1) {
            sem_wait(&db);  // First reader locks the database
        }
        
        sem_post(&s);  // Release the semaphore to allow other readers to proceed

        // Perform reading
        readDatabase(reader);

        // Enter critical section to update rc again
        sem_wait(&s);

        // If this is the last reader, release the database lock
        rc--;
        if (rc == 0) {
            sem_post(&db);  // Last reader unlocks the database
        }

        sem_post(&s);  // Release the semaphore

        // Use the database
        useDatabase(reader);
    }
    return NULL;
}

void *writer(void *arg) {
    while (1) {
        thinkUpdata();
        
        // Wait for exclusive access to the database (lock the database)
        sem_wait(&db);

        // Perform writing
        writeDatabase();

        // Release the database lock
        sem_post(&db);
    }
    return NULL;
}

int main() {
    // Initialize semaphores
    sem_init(&s, 0, 1);  // Semaphore for controlling read count access
    sem_init(&db, 0, 1);  // Semaphore for controlling access to the database

    pthread_t reader_threads[3], writer_thread;
    int reader_ids[3];
    
    for(int i =0 ;i <3; i++){
        reader_ids[i]=i;
    }

    // Create multiple reader threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&reader_threads[i], NULL, reader, &reader_ids[i]);
    }

    // Create a writer thread
    pthread_create(&writer_thread, NULL, writer, NULL);

    // Wait for threads to finish (this will never happen in the current infinite loop)
    for (int i = 0; i < 3; i++) {
        pthread_join(reader_threads[i], NULL);
    }
    pthread_join(writer_thread, NULL);

    // Clean up semaphores
    sem_destroy(&s);
    sem_destroy(&db);

    return 0;
}