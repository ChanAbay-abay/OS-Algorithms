#include <stdio.h>
#include <limits.h> // For INT_MAX
#define MAX 10

struct Process {
    int id;
    int arrival;
    int burst;
    int priority;
    int completion;
    int turnaround;
    int waiting;
    int remainingBurst; // For preemptive scheduling
};

struct ProcList {
    struct Process Proc[MAX];
    int processCount;
    float avgTurnaround;
    float avgWaiting;
};

// Finds the next process to run based on highest priority that has arrived
int findNextProcess(struct ProcList *pList, int currentTime) {
    int highestPriorityIndex = -1;
    int highestPriority = INT_MIN;
    for (int i = 0; i < pList->processCount; i++) {
        if (pList->Proc[i].arrival <= currentTime && pList->Proc[i].remainingBurst > 0) {
            if (pList->Proc[i].priority > highestPriority) {
                highestPriority = pList->Proc[i].priority;
                highestPriorityIndex = i;
            }
        }
    }
    return highestPriorityIndex;
}

// Run the preemptive priority scheduling simulation
int runPreemptivePriorityScheduling(struct ProcList *pList, int timeline[], int max_possible_time) {
    int completed = 0;
    int currentTime = 0;
    int totalProcesses = pList->processCount;

    // Initialize remaining burst times
    for (int i = 0; i < totalProcesses; i++) {
        pList->Proc[i].remainingBurst = pList->Proc[i].burst;
    }

    // Initialize timeline with idle state (-1)
    for (int i = 0; i < max_possible_time; i++) {
        timeline[i] = -1;
    }

    while (completed < totalProcesses) {
        int nextProc = findNextProcess(pList, currentTime);

        if (nextProc == -1) {
            // No process is ready, CPU idle
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

    return currentTime; // Return the last time unit executed
}

// Calculate turnaround times for all processes
void findTurnaroundTimes(struct ProcList *pList) {
    for (int i = 0; i < pList->processCount; i++) {
        pList->Proc[i].turnaround = pList->Proc[i].completion - pList->Proc[i].arrival;
    }
}

// Calculate waiting times for all processes
void findWaitingTimes(struct ProcList *pList) {
    for (int i = 0; i < pList->processCount; i++) {
        pList->Proc[i].waiting = pList->Proc[i].turnaround - pList->Proc[i].burst;
    }
}

// Calculate average turnaround and waiting times
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

// Print the Gantt chart
void printGanttChart(struct ProcList *pList, int timeline[], int endTime) {
    // Find max completion time
    int maxCompletionTime = 0;
    for (int i = 0; i < pList->processCount; i++) {
        if (pList->Proc[i].completion > maxCompletionTime) {
            maxCompletionTime = pList->Proc[i].completion;
        }
    }

    printf("\nGantt Chart (Preemptive Priority):\n");
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
                // Running slot
                printf("==== ");
            } else {
                // Not running
                printf("     ");
            }
        }
        printf("(%d)\n", pList->Proc[i].completion);
    }
}

// Print the process summary table
void printProcessSummary(struct ProcList *pList) {
    printf("\nProcess Summary:\n");
    printf("-------------------------------------------------------------------\n");
    printf("| ID | Arrival | Burst | Priority | Completion | Turnaround | Waiting |\n");
    printf("|----|----------|-------|----------|------------|------------|---------|\n");
    for (int i = 0; i < pList->processCount; i++) {
        printf("| %2d |    %2d    |  %2d   |    %2d    |     %2d     |     %2d     |   %2d    |\n",
               pList->Proc[i].id,
               pList->Proc[i].arrival,
               pList->Proc[i].burst,
               pList->Proc[i].priority,
               pList->Proc[i].completion,
               pList->Proc[i].turnaround,
               pList->Proc[i].waiting);
    }
    printf("-------------------------------------------------------------------\n");

    printf("Avg Turnaround Time: %.2f\n", pList->avgTurnaround);
    printf("Avg Waiting Time: %.2f\n", pList->avgWaiting);
}

int main() {
    struct ProcList procList;
    procList.processCount = 0;

    printf("========= Preemptive Priority Scheduling =========\n\n");
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
        printf("Enter priority for process %d (Higher number = Higher priority): ", procList.Proc[i].id);
        scanf("%d", &procList.Proc[i].priority);
        procList.Proc[i].completion = 0;
    }

    // Timeline array to track which process runs at each time unit
    // Set a large enough size for demonstration purposes.
    // In a real scenario, you may limit based on sum of all burst times.
    int timeline[10000];

    int lastTime = runPreemptivePriorityScheduling(&procList, timeline, 10000);
    findTurnaroundTimes(&procList);
    findWaitingTimes(&procList);
    findAvgTimes(&procList);
    printGanttChart(&procList, timeline, lastTime);
    printProcessSummary(&procList);

    return 0;
}