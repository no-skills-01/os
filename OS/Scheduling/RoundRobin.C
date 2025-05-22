#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define SIZE 100

struct Process {
    int pid;
    int at;
    int bt;
    int rt;
    int ft;
    int wt;
    int tt;
};

void printTimeline(int *Timeline, int time) {
    char ch;
    printf("Timeline : ");
    for (int i = 0; i < time; i++) {
        if (Timeline[i] == -1) {
            printf("Idle  ");
            continue;
        }
        ch = (char)(Timeline[i] + 65);
        printf("%c  ", ch);
    }
    printf("\n\n");
}

int main() {
    int n, tq;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter time quantum: ");
    scanf("%d", &tq);

    struct Process *p = (struct Process *)malloc(n * sizeof(struct Process));
    int time = 0, total_bt = 0;

    for (int i = 0; i < n; i++) {
        p[i].pid = i;
        printf("Enter AT and BT of process %c: ", i + 65);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].rt = p[i].bt;
        total_bt += p[i].bt;
    }

    int *Timeline = (int *)malloc((total_bt + 100) * sizeof(int));
    int *Queue = (int *)malloc(SIZE * sizeof(int));
    int front = 0, rear = -1;
    int executing = -1, completed = 0, i = 0;
    int *visited = (int *)calloc(n, sizeof(int));

    while (completed < n) {
        // Add newly arrived processes
        for (int j = 0; j < n; j++) {
            if (p[j].at == i && visited[j] == 0) {
                Queue[++rear] = j;
                visited[j] = 1;
            }
        }

        if (front > rear && executing == -1) {
            Timeline[i] = -1; // CPU idle
            i++;
            continue;
        }

        if (executing == -1 && front <= rear) {
            executing = Queue[front++];
        }

        int execTime = (p[executing].rt < tq) ? p[executing].rt : tq;

        for (int t = 0; t < execTime; t++) {
            Timeline[i++] = executing;

            // Add newly arrived during execution
            for (int j = 0; j < n; j++) {
                if (p[j].at == i && visited[j] == 0) {
                    Queue[++rear] = j;
                    visited[j] = 1;
                }
            }

            p[executing].rt--;
            if (p[executing].rt == 0) break;
        }

        if (p[executing].rt == 0) {
            p[executing].ft = i;
            completed++;
            executing = -1;
        } else {
            Queue[++rear] = executing; // re-enqueue
            executing = -1;
        }
    }

    printTimeline(Timeline, i);

    float avgtt = 0, avgwt = 0;

    for (int i = 0; i < n; i++) {
        p[i].tt = p[i].ft - p[i].at;
        p[i].wt = p[i].tt - p[i].bt;
        avgtt += p[i].tt;
        avgwt += p[i].wt;
    }

    avgtt /= n;
    avgwt /= n;

    printf("\nFinal Outcome :\n\n");
    printf("PID\tAT\tBT\tFT\tTT\tWT\n\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ft, p[i].tt, p[i].wt);
    }

    printf("\nAverage Turnaround Time -> %.2f\n", avgtt);
    printf("Average Waiting Time -> %.2f\n", avgwt);

    printf("\nPress Enter to exit...");
    getchar(); getchar();

    free(p);
    free(Timeline);
    free(Queue);
    free(visited);

    return 0;
}
