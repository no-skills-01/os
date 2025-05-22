#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

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
    printf("\nTimeline :\n");
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
    printf("Enter number of processes\n");
    int n;
    scanf("%d", &n);

    struct Process *p = (struct Process *)malloc(n * sizeof(struct Process));

    for (int i = 0; i < n; i++) {
        p[i].pid = i;
        printf("Enter AT and BT of process %c\n", i + 65);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].rt = p[i].bt;
    }

    int *Queue = (int *)malloc(n * sizeof(int));
    int *Timeline = (int *)malloc(1000 * sizeof(int));  // Allocate enough space

    int executing = -1;
    int completed = 0;
    int i = 0;
    int qptre = -1, qptrb = 0;

    while (completed < n) {

        // Add arriving processes
        for (int j = 0; j < n; j++) {
            if (i == p[j].at) {
                if (executing == -1) {
                    executing = p[j].pid;
                } else {
                    if (p[j].bt < p[executing].rt) {
                        Queue[++qptre] = executing;
                        executing = p[j].pid;
                    } else {
                        Queue[++qptre] = p[j].pid;
                    }
                }
            }
        }

        // CPU idle
        if (executing == -1 && qptrb > qptre) {
            Timeline[i++] = -1;
            continue;
        }

        // Load from queue
        if (executing == -1 && qptrb <= qptre) {
            executing = Queue[qptrb++];
        }

        Timeline[i] = executing;
        p[executing].rt--;

        if (p[executing].rt == 0) {
            p[executing].ft = i + 1;
            executing = -1;
            completed++;
        }

        i++;
    }

    printTimeline(Timeline, i);  // ✅ Use actual length of timeline

    float avgtt = 0, avgwt = 0;

    for (int i = 0; i < n; i++) {
        p[i].tt = p[i].ft - p[i].at;
        p[i].wt = p[i].tt - p[i].bt;
        avgtt += p[i].tt;
        avgwt += p[i].wt;
    }

    avgtt /= n;
    avgwt /= n;

    printf("Final Outcome:\n\n");
    printf("PID\tAT\tBT\tFT\tTT\tWT\n\n");

    for (int i = 0; i < n; i++) {
        printf("%c\t%d\t%d\t%d\t%d\t%d\n", i + 65, p[i].at, p[i].bt, p[i].ft, p[i].tt, p[i].wt);
    }

    printf("\nAverage Turnaround Time -> %.2f\nAverage Waiting Time -> %.2f\n", avgtt, avgwt);
    printf("\nPress Enter to exit...\n");

    getchar();
    getchar();

    return 0;
}
