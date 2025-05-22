#include <stdio.h>
#include <stdlib.h>

int fcfs_diskScheduling(int *requests, int n, int initial_Position) {
    int total_seek_time = 0;
    int current_Position = initial_Position;

    for (int i = 0; i < n; i++) {
        int seek_distance = abs(requests[i] - current_Position);
        total_seek_time += seek_distance;
        current_Position = requests[i];
    }

    return total_seek_time;
}

int main() {
    int n;
    printf("Enter the number of Requests you want in the Request Queue: ");
    scanf("%d", &n);

    int *requests = (int *)malloc(n * sizeof(int));
    if (requests == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    printf("Enter the requests: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &requests[i]);
    }

    int initial_Position;
    printf("Enter the initial position of head: ");
    scanf("%d", &initial_Position);

    int seek_time = fcfs_diskScheduling(requests, n, initial_Position);

    printf("The total seek time using FCFS is: %d\n", seek_time);

    free(requests); // Free dynamically allocated memory

    return 0;
}
