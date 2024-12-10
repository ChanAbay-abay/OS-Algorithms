#include <stdio.h>
#include <limits.h> // For INT_MAX
#define MAX 10

struct Process {
    int id;
    int arrival;
    int burst;
    int completion;
    int turnaround;
    int waiting;
    int remainingBurst;
};

struct ProcList {
    struct Process Proc[MAX];
    int processCount;
    float avgTurnaround;
    float avgWaiting;
};

// Find the process with the shortest remaining time that is ready at currentTime
int findNextProcessSRTF(struct ProcList *pList, int currentTime) {
    int shortestIndex = -1;
    int shortestRemaining = INT_MAX;

    for (int i = 0; i < pList->processCount; i++) {
        if (pList->Proc[i].arrival <= currentTime && pList->Proc[i].remainingBurst > 0) {
            if (pList->Proc[i].remainingBurst < shortestRemaining) {
                shortestRemaining = pList->Proc[i].remainingBurst;
                shortestIndex = i;
            }
        }
    }

    return shortestIndex;
}

// Run the SRTF scheduling simulation
int runSRTF(struct ProcList *pList, int timeline[], int max_possible_time) {
    int completed = 0;
    int currentTime = 0;
    int totalProcesses = pList->processCount;

    // Initialize remaining burst times
    for (int i = 0; i < totalProcesses; i++) {
        pList->Proc[i].remainingBurst = pList->Proc[i].burst;
    }

    // Initialize timeline
    for (int i = 0; i < max_possible_time; i++) {
        timeline[i] = -1;
    }

    // Continue until all processes are completed
    while (completed < totalProcesses) {
        int nextProc = findNextProcessSRTF(pList, currentTime);

        if (nextProc == -1) {
            // No process ready, CPU idle
            timeline[currentTime] = -1;
            currentTime++;
        } else {
            // Run the selected process for one unit of time
            pList->Proc[nextProc].remainingBurst -= 1;
            timeline[currentTime] = pList->Proc[nextProc].id;
            currentTime++;

            // If the process finishes at this time
            if (pList->Proc[nextProc].remainingBurst == 0) {
                pList->Proc[nextProc].completion = currentTime;
                completed++;
            }
        }
    }

    return currentTime; // last time unit used
}

void findTurnaroundTimes(struct ProcList *pList) {
    for (int i = 0; i < pList->processCount; i++) {
        pList->Proc[i].turnaround = pList->Proc[i].completion - pList->Proc[i].arrival;
    }
}

void findWaitingTimes(struct ProcList *pList) {
    for (int i = 0; i < pList->processCount; i++) {
        pList->Proc[i].waiting = pList->Proc[i].turnaround - pList->Proc[i].burst;
    }
}

void findAvgTimes(struct ProcList *pList) {
    float totalTurnaround = 0.0;
    float totalWaiting = 0.0;
    for (int i = 0; i < pList->processCount; i++) {
        totalTurnaround += pList->Proc[i].turnaround;
        totalWaiting += pList->Proc[i].waiting;
    }
    pList->avgTurnaround = totalTurnaround / pList->processCount;
    pList->avgWaiting = totalWaiting / pList->processCount;
}

void printGanttChart(struct ProcList *pList, int timeline[], int endTime) {
    // Find max completion time
    int maxCompletionTime = 0;
    for (int i = 0; i < pList->processCount; i++) {
        if (pList->Proc[i].completion > maxCompletionTime) {
            maxCompletionTime = pList->Proc[i].completion;
        }
    }

    printf("\nGantt Chart (Shortest Remaining Time First):\n");
    // Time axis
    printf("Time:    ");
    for (int t = 0; t <= maxCompletionTime; t++) {
        printf("%-3d ", t);
    }
    printf("\n");

    // For each process, show when it was running
    for (int i = 0; i < pList->processCount; i++) {
        printf("P%d:     ", pList->Proc[i].id);
        for (int t = 0; t <= maxCompletionTime; t++) {
            if (t < maxCompletionTime && timeline[t] == pList->Proc[i].id) {
                printf("==== ");
            } else {
                printf("     ");
            }
        }
        printf("(%d)\n", pList->Proc[i].completion);
    }
}

void printProcessSummary(struct ProcList *pList) {
    printf("\nProcess Summary:\n");
    printf("---------------------------------------------------------\n");
    printf("| ID | Arrival | Burst | Completion | Turnaround | Waiting |\n");
    printf("|----|----------|-------|------------|------------|---------|\n");
    for (int i = 0; i < pList->processCount; i++) {
        printf("| %2d |    %2d    |  %2d   |     %2d     |     %2d     |   %2d    |\n",
               pList->Proc[i].id,
               pList->Proc[i].arrival,
               pList->Proc[i].burst,
               pList->Proc[i].completion,
               pList->Proc[i].turnaround,
               pList->Proc[i].waiting);
    }
    printf("---------------------------------------------------------\n");

    printf("Avg Turnaround Time: %.2f\n", pList->avgTurnaround);
    printf("Avg Waiting Time: %.2f\n", pList->avgWaiting);
}

int main() {
    struct ProcList procList;
    procList.processCount = 0;

    printf("========= Shortest Remaining Time First (SRTF) =========\n\n");
    printf("Enter number of processes: ");
    scanf("%d", &procList.processCount);

    if (procList.processCount < 1) {
        printf("\nNo processes to schedule!\nProgram Terminated.\n");
        return 0;
    }

    for (int i = 0; i < procList.processCount; i++) {
        procList.Proc[i].id = i + 1;
        printf("\nEnter arrival time for process %d: ", procList.Proc[i].id);
        scanf("%d", &procList.Proc[i].arrival);
        printf("Enter burst time for process %d: ", procList.Proc[i].id);
        scanf("%d", &procList.Proc[i].burst);
        procList.Proc[i].completion = 0; 
    }

    // Timeline array to track which process runs at each time unit
    int timeline[10000];

    int lastTime = runSRTF(&procList, timeline, 10000);
    findTurnaroundTimes(&procList);
    findWaitingTimes(&procList);
    findAvgTimes(&procList);
    printGanttChart(&procList, timeline, lastTime);
    printProcessSummary(&procList);

    return 0;
}