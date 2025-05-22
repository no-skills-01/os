#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Process {
    int pid, at, bt, rt, ft, tt, wt, priority;
    int completed;
};

// 👇 Modularized timeline printer
void printTimeline(int *Timeline, int time) {
    printf("\nTimeline: ");
    for (int i = 0; i < time; i++) {
        if (Timeline[i] == -1)
            printf("Idle ");
        else
            printf("%c ", Timeline[i] + 65);
    }
    printf("\n");
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    int total_bt = 0;

    for (int i = 0; i < n; i++) {
        p[i].pid = i;
        printf("Enter AT, BT, and Priority of process %c: ", i + 65);
        scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].priority);
        p[i].rt = p[i].bt;
        p[i].completed = 0;
        total_bt += p[i].bt;
    }

    int time = 0, completed = 0;
    int *Timeline = (int*)malloc((total_bt + 100) * sizeof(int)); // extra buffer

    while (completed < n) {
        int idx = -1, highestPriority = 9999;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0 && p[i].priority < highestPriority) {
                highestPriority = p[i].priority;
                idx = i;
            }
        }

        if (idx != -1) {
            p[idx].rt--;
            Timeline[time] = idx;

            if (p[idx].rt == 0) {
                p[idx].ft = time + 1;
                p[idx].tt = p[idx].ft - p[idx].at;
                p[idx].wt = p[idx].tt - p[idx].bt;
                p[idx].completed = 1;
                completed++;
            }
        } else {
            Timeline[time] = -1;  // CPU idle
        }

        time++;
    }

    // 🔹 Print execution order (Gantt-style)
    printTimeline(Timeline, time);

    // 🔹 Print process table
    printf("\nPID\tAT\tBT\tP\tFT\tTT\tWT\n");
    float avgtt = 0, avgwt = 0;
    for (int i = 0; i < n; i++) {
        printf("%c\t%d\t%d\t%d\t%d\t%d\t%d\n", i + 65, p[i].at, p[i].bt, p[i].priority,
               p[i].ft, p[i].tt, p[i].wt);
        avgtt += p[i].tt;
        avgwt += p[i].wt;
    }

    // 🔹 Print averages
    printf("\nAverage Turnaround Time: %.2f\n", avgtt / n);
    printf("Average Waiting Time: %.2f\n", avgwt / n);

    free(Timeline);
    return 0;
}
