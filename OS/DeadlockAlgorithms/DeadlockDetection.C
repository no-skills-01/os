#include <stdio.h>
#include <stdbool.h>

#define MAX 100

int available[MAX];  // Available instances for each resource
int allocation[MAX][MAX];  // Allocation matrix (P x R)
int request[MAX][MAX];  // Request matrix (P x R)

bool isSafeState(int P, int R) {
    int work[MAX];
    bool finish[MAX] = {false};
    int safeSeq[MAX];
    int count = 0;

    // Initialize work with available resources
    for (int i = 0; i < R; i++) {
        work[i] = available[i];
    }

    while (count < P) {
        bool progress = false;

        // Find a process that can execute
        for (int i = 0; i < P; i++) {
            if (!finish[i]) {
                bool canExecute = true;

                // Check if the process's request is less than or equal to the available resources
                for (int j = 0; j < R; j++) {
                    if (request[i][j] > work[j]) {
                        canExecute = false;
                        break;
                    }
                }

                // If the process can execute, simulate it
                if (canExecute) {
                    finish[i] = true;
                    safeSeq[count++] = i;
                    progress = true;

                    // Release resources allocated to process i
                    for (int j = 0; j < R; j++) {
                        work[j] += allocation[i][j];
                    }

                    printf("\n");
                    printf("Process P%d executed. Released resources.\n", i);
                    break;
                }
            }
        }

        // If no progress can be made, the system is in an unsafe state
        if (!progress) {
            return false;
        }
    }

    // If we finished all processes, print the safe sequence
    printf("\nSafe execution sequence: ");
    for (int i = 0; i < P; i++) {
        printf("P%d ", safeSeq[i]);
    }
    printf("\n");

    return true;
}

int main() {
    int P, R;

    printf("Enter number of processes: ");
    scanf("%d", &P);

    printf("Enter number of resources: ");
    scanf("%d", &R);

    // Taking input for the available instances of resources
    printf("\nEnter Available Resources (R):\n");
    for (int i = 0; i < R; i++) {
        printf("Enter available instances for Resource R%d: ", i);
        scanf("%d", &available[i]);
    }

    // Taking input for the Allocation Matrix (P x R)
    printf("\nEnter Allocation Matrix (P x R):\n");
    for (int i = 0; i < P; i++) {
        printf("For Process P%d:\n", i);
        for (int j = 0; j < R; j++) {
            printf("Enter allocation for Resource R%d: ", j);
            scanf("%d", &allocation[i][j]);
        }
    }

    // Taking input for the Request Matrix (P x R)
    printf("\nEnter Request Matrix (P x R):\n");
    for (int i = 0; i < P; i++) {
        printf("For Process P%d:\n", i);
        for (int j = 0; j < R; j++) {
            printf("Enter request for Resource R%d: ", j);
            scanf("%d", &request[i][j]);
        }
    }

    // Perform safe state check using Banker's Algorithm
    if (isSafeState(P, R)) {
        printf("\nSystem is in a safe state.\n");
        printf("\nNo DeadLock.\n");
    } else {
        printf("\nSystem is in an unsafe state.\n");
    }

    return 0;
}