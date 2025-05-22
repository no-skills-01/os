#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int P;  // Number of processes
int R;  // Number of resources
int **allocation;
int **request;
int *available;

void initializeArrays() {
    allocation = (int **)malloc(P * sizeof(int *));
    request = (int **)malloc(P * sizeof(int *));
    available = (int *)malloc(R * sizeof(int));
    
    for(int i = 0; i < P; i++) {
        allocation[i] = (int *)malloc(R * sizeof(int));
        request[i] = (int *)malloc(R * sizeof(int));
    }
}

void inputData() {
    printf("\nEnter allocation matrix:\n");
    for(int i = 0; i < P; i++) {
        printf("For Process %d:\n", i);
        for(int j = 0; j < R; j++) {
            printf("Resource %d: ", j);
            scanf("%d", &allocation[i][j]);
        }
    }

    printf("\nEnter request matrix:\n");
    for(int i = 0; i < P; i++) {
        printf("For Process %d:\n", i);
        for(int j = 0; j < R; j++) {
            printf("Resource %d: ", j);
            scanf("%d", &request[i][j]);
        }
    }

    printf("\nEnter available resources:\n");
    for(int i = 0; i < R; i++) {
        printf("Resource %d: ", i);
        scanf("%d", &available[i]);
    }
}

bool deadlockDetection() {
    bool *finish = (bool *)malloc(P * sizeof(bool));
    int *work = (int *)malloc(R * sizeof(int));

    for (int i = 0; i < P; i++)
        finish[i] = false;
    for (int i = 0; i < R; i++)
        work[i] = available[i];

    while (true) {
        bool found = false;
        for (int i = 0; i < P; i++) {
            if (!finish[i]) {
                bool canProceed = true;
                for (int j = 0; j < R; j++) {
                    if (request[i][j] > work[j]) {
                        canProceed = false;
                        break;
                    }
                }

                if (canProceed) {
                    for (int j = 0; j < R; j++)
                        work[j] += allocation[i][j];
                    finish[i] = true;
                    found = true;
                }
            }
        }

        if (!found)
            break;
    }

    bool deadlock = false;
    for (int i = 0; i < P; i++) {
        if (!finish[i]) {
            printf("Process P%d is deadlocked.\n", i);
            deadlock = true;
        }
    }

    if (!deadlock)
        printf("No deadlock detected.\n");

    free(finish);
    free(work);
    return deadlock;
}

void freeMemory() {
    for(int i = 0; i < P; i++) {
        free(allocation[i]);
        free(request[i]);
    }
    free(allocation);
    free(request);
    free(available);
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &P);
    printf("Enter number of resources: ");
    scanf("%d", &R);

    initializeArrays();
    inputData();
    deadlockDetection();
    freeMemory();
    return 0;
}
