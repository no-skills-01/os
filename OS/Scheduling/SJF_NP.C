#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid, at, bt, ct, tat, wt;
};

// 🔹 Timeline printer
void printTimeline(int Timeline[], int length) {
    printf("\nTimeline: ");
    for (int i = 0; i < length; i++) {
        if (Timeline[i] == -1)
            printf("Idle ");
        else
            printf("P%d ", Timeline[i]);
    }
    printf("\n");
}

void sjf_non_preemptive(struct Process p[], int n) {
    int completed = 0, time = 0, min_bt, index;
    int is_completed[n];
    int Timeline[1000]; // large enough for most use cases
    int timeline_index = 0;

    for (int i = 0; i < n; i++)
        is_completed[i] = 0; // Initially, no process is completed

    while (completed < n) {
        min_bt = 9999;
        index = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && !is_completed[i] && p[i].bt < min_bt) {
                min_bt = p[i].bt;
                index = i;
            }
        }

        if (index == -1) {
            Timeline[timeline_index++] = -1; // idle
            time++;
            continue;
        }

        for (int t = 0; t < p[index].bt; t++) {
            Timeline[timeline_index++] = p[index].pid; // store each unit of process execution
        }

        p[index].ct = time + p[index].bt;
        p[index].tat = p[index].ct - p[index].at;
        p[index].wt = p[index].tat - p[index].bt;
        time = p[index].ct;
        is_completed[index] = 1;
        completed++;
    }

    printTimeline(Timeline, timeline_index); // ⬅️ Print Gantt-style chart
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time & Burst Time for P%d: ", p[i].pid);
        scanf("%d %d", &p[i].at, &p[i].bt);
    }

    sjf_non_preemptive(p, n);

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);

    return 0;
}
