#include <stdio.h>
#define MAX 10 // Maximum number of processes

struct Process { // Each process
    int id;         // Process ID
    int waiting;    // Waiting time
    int arrival;    // Arrival time
    int completion; // Completion time
    int burst;      // Burst time
    int turnaround; // Turnaround time
};

struct ProcList { // List of processes
    struct Process Proc[MAX];
    int processCount;
    float avgTurnaround; // Average turnaround time
    float avgWaiting;    // Average waiting time
};

void findCompletionTime(struct ProcList *pList) {
    int currentTime = 0; // Keeps track of current time in scheduling
    int completed = 0;   // Tracks the number of completed processes

    while (completed < pList->processCount) {
        int shortestJob = -1;
        int shortestBurst = 1e9; // Use a large initial value for comparison

        // Find the process with the shortest burst time that has arrived
        for (int i = 0; i < pList->processCount; i++) {
            // Check if the process has arrived and not yet completed
            if (pList->Proc[i].arrival <= currentTime && pList->Proc[i].completion == 0) {
                if (pList->Proc[i].burst < shortestBurst) {
                    shortestBurst = pList->Proc[i].burst;
                    shortestJob = i;
                }
            }
        }

        if (shortestJob == -1) {
            // No process arrived yet, so move time forward
            currentTime++;
        } else {
            // Process the selected shortest job
            pList->Proc[shortestJob].completion = currentTime + pList->Proc[shortestJob].burst;
            currentTime = pList->Proc[shortestJob].completion;
            completed++;
        }
    }
}

void findTurnaroundTime(struct ProcList *pList) {
    for (int i = 0; i < pList->processCount; i++) {
        pList->Proc[i].turnaround = pList->Proc[i].completion - pList->Proc[i].arrival;
    }
}

void findWaitingTime(struct ProcList *pList) {
    for (int i = 0; i < pList->processCount; i++) {
        pList->Proc[i].waiting = pList->Proc[i].turnaround - pList->Proc[i].burst;
    }
}

void findAvgTime(struct ProcList *pList) {
    float totalTurnaround = 0, totalWaiting = 0;
    for (int i = 0; i < pList->processCount; i++) {
        totalTurnaround += pList->Proc[i].turnaround;
        totalWaiting += pList->Proc[i].waiting;
    }
    pList->avgTurnaround = totalTurnaround / pList->processCount;
    pList->avgWaiting = totalWaiting / pList->processCount;
}

void printGanttChart(struct ProcList *pList) {
    if (pList->processCount > 0) {
        printf("\nGantt Chart: (NOTE: May need to make Terminal Wider for longer Process Lists)\n");
        printf("Waiting: '----', Executing: '===='\n");

        // Determine the maximum completion time
        int maxCompletionTime = 0;
        for (int i = 0; i < pList->processCount; i++) {
            if (pList->Proc[i].completion > maxCompletionTime) {
                maxCompletionTime = pList->Proc[i].completion;
            }
        }

        // Print header row for time
        printf("Time:   ");
        for (int time = 0; time <= maxCompletionTime; time++) {
            printf("%-4d ", time);
        }
        printf("\n");

        // Print process rows with arrival (waiting) and execution bars
        for (int i = 0; i < pList->processCount; i++) {
            printf("P%d:    ", pList->Proc[i].id);
            for (int time = 0; time <= maxCompletionTime; time++) {
                if (time >= pList->Proc[i].arrival && time < pList->Proc[i].completion) {
                    if (time < pList->Proc[i].arrival + pList->Proc[i].waiting) {
                        // Print waiting time as "----"
                        printf("---- ");
                    } else {
                        // Print execution time as "===="
                        printf("==== ");
                    }
                } else {
                    // Empty spaces for time slots not related to this process
                    printf("     ");
                }
            }
            printf(" (%d)", pList->Proc[i].completion);
            printf("\n");
        }

        // Process summary table
        printf("\nProcess Summary Table:\n");
        printf("-------------------------------------------------------------\n");
        printf("| ID  | Arrival | Burst | Completion | Turnaround | Waiting |\n");
        printf("|-----|---------|-------|------------|------------|---------|\n");
        for (int i = 0; i < pList->processCount; i++) {
            printf("| %2d  |   %2d    |  %2d   |     %2d     |     %2d     |   %2d    |\n",
                pList->Proc[i].id,
                pList->Proc[i].arrival,
                pList->Proc[i].burst,
                pList->Proc[i].completion,
                pList->Proc[i].turnaround,
                pList->Proc[i].waiting);
        }
        printf("-------------------------------------------------------------\n");

        printf("Avg Turnaround Time: %.2f\n", pList->avgTurnaround);
        printf("Avg Waiting Time: %.2f\n", pList->avgWaiting);
    }
}

int main() {
    struct ProcList procList;
    procList.processCount = 0;

    printf("========= Shortest Job First =========\n\n");
    printf("Enter number of processes: ");
    scanf("%d", &procList.processCount);

    if (procList.processCount < 1) {
        printf("\nThere are no processes to calculate for!\nProgram Terminated.");
    } else {
        for (int i = 0; i < procList.processCount; i++) {
            procList.Proc[i].id = i + 1;
            printf("\nEnter arrival time for process %d: ", procList.Proc[i].id);
            scanf("%d", &procList.Proc[i].arrival);
            printf("Enter burst time for process %d: ", procList.Proc[i].id);
            scanf("%d", &procList.Proc[i].burst);
            procList.Proc[i].completion = 0; // Initialize completion time
        }

        findCompletionTime(&procList);
        findTurnaroundTime(&procList);
        findWaitingTime(&procList);
        findAvgTime(&procList);
        printGanttChart(&procList);
    }

    return 0;
}