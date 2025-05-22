#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid, at, bt, ft, tt, wt, priority;
    int completed;
};

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].pid = i;
        printf("Enter AT, BT, and Priority of process %c: ", i + 65);
        scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].priority);
        p[i].completed = 0;
    }

    int time = 0, completed = 0;
    while (completed < n) {
        int idx = -1, highestPriority = 9999;

        for (int i = 0; i < n; i++) {
            if (!p[i].completed && p[i].at <= time && p[i].priority < highestPriority) {
                highestPriority = p[i].priority;
                idx = i;
            }
        }

        if (idx != -1) {
            time += p[idx].bt;
            p[idx].ft = time;
            p[idx].tt = p[idx].ft - p[idx].at;
            p[idx].wt = p[idx].tt - p[idx].bt;
            p[idx].completed = 1;
            completed++;
        } else {
            time++;
        }
    }

    printf("\nPID\tAT\tBT\tP\tFT\tTT\tWT\n");
    float avgtt = 0, avgwt = 0;
    for (int i = 0; i < n; i++) {
        printf("%c\t%d\t%d\t%d\t%d\t%d\t%d\n", i + 65, p[i].at, p[i].bt, p[i].priority,
               p[i].ft, p[i].tt, p[i].wt);
        avgtt += p[i].tt;
        avgwt += p[i].wt;
    }

    printf("\nAverage Turnaround Time: %.2f\n", avgtt / n);
    printf("Average Waiting Time: %.2f\n", avgwt / n);
    return 0;
}
