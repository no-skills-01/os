#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int P;  // Number of processes
int R;  // Number of resources
int *available;
int **max;
int **allocation;
int **need;

void initializeArrays() {
    // Allocate memory for arrays
    available = (int *)malloc(R * sizeof(int));
    
    max = (int **)malloc(P * sizeof(int *));
    allocation = (int **)malloc(P * sizeof(int *));
    need = (int **)malloc(P * sizeof(int *));
    
    for(int i = 0; i < P; i++) {
        max[i] = (int *)malloc(R * sizeof(int));
        allocation[i] = (int *)malloc(R * sizeof(int));
        need[i] = (int *)malloc(R * sizeof(int));
    }
}

void inputData() {
    printf("Enter available resources:\n");
    for(int i = 0; i < R; i++) {
        printf("Resource %d: ", i);
        scanf("%d", &available[i]);
    }

    printf("\nEnter maximum resource matrix:\n");
    for(int i = 0; i < P; i++) {
        printf("For Process %d:\n", i);
        for(int j = 0; j < R; j++) {
            printf("Resource %d: ", j);
            scanf("%d", &max[i][j]);
        }
    }

    printf("\nEnter allocation matrix:\n");
    for(int i = 0; i < P; i++) {
        printf("For Process %d:\n", i);
        for(int j = 0; j < R; j++) {
            printf("Resource %d: ", j);
            scanf("%d", &allocation[i][j]);
        }
    }
}

void calculateNeed() {
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - allocation[i][j];
}

bool isSafe() {
    int *work = (int *)malloc(R * sizeof(int));
    bool *finish = (bool *)malloc(P * sizeof(bool));

    for (int i = 0; i < R; i++)
        work[i] = available[i];
    for (int i = 0; i < P; i++)
        finish[i] = false;

    int *safeSeq = (int *)malloc(P * sizeof(int));
    int count = 0;

    while (count < P) {
        bool found = false;
        for (int i = 0; i < P; i++) {
            if (!finish[i]) {
                bool canProceed = true;
                for (int j = 0; j < R; j++) {
                    if (need[i][j] > work[j]) {
                        canProceed = false;
                        break;
                    }
                }

                if (canProceed) {
                    for (int j = 0; j < R; j++)
                        work[j] += allocation[i][j];
                    safeSeq[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            printf("System is not in a safe state.\n");
            free(work);
            free(finish);
            free(safeSeq);
            return false;
        }
    }

    printf("System is in a safe state.\nSafe sequence: ");
    for (int i = 0; i < P; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");

    free(work);
    free(finish);
    free(safeSeq);
    return true;
}

void freeMemory() {
    for(int i = 0; i < P; i++) {
        free(max[i]);
        free(allocation[i]);
        free(need[i]);
    }
    free(max);
    free(allocation);
    free(need);
    free(available);
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &P);
    printf("Enter number of resources: ");
    scanf("%d", &R);

    initializeArrays();
    inputData();
    calculateNeed();
    isSafe();
    freeMemory();
    return 0;
}
