#include <stdio.h>
#define MAX 10 // Maximum number of processes

struct Process {
    int id;         // Process ID
    int arrival;    // Arrival time
    int burst;      // Burst time
    int remaining;  // Remaining burst time for preemption
    int completion; // Completion time
    int waiting;    // Waiting time
    int turnaround; // Turnaround time
};

struct ProcList {
    struct Process Proc[MAX];
    int processCount;
    float avgTurnaround; // Average turnaround time
    float avgWaiting;    // Average waiting time
};

void roundRobin(struct ProcList *pList, int quantum) {
    int currentTime = 0; // Current time in the scheduler
    int completed = 0;   // Number of completed processes

    // Run the scheduling until all processes are completed
    while (completed < pList->processCount) {
        int progressMade = 0; // Flag to check if any process has been executed

        // Round Robin Process Execution
        for (int i = 0; i < pList->processCount; i++) {
            if (pList->Proc[i].arrival <= currentTime && pList->Proc[i].remaining > 0) {
                // Determine the time slice for the current process
                int timeSlice = (pList->Proc[i].remaining > quantum) ? quantum : pList->Proc[i].remaining;
                pList->Proc[i].remaining -= timeSlice;
                currentTime += timeSlice;
                progressMade = 1;

                // If the process has completed, set the completion time
                if (pList->Proc[i].remaining == 0) {
                    pList->Proc[i].completion = currentTime;
                    completed++;
                }
            }
        }

        if (!progressMade) {
            currentTime++; // If no process made progress, increment time
        }
    }
}

void findTurnaroundTime(struct ProcList *pList) {
    for (int i = 0 ; i < pList->processCount ; i++) {
        pList->Proc[i].turnaround = pList->Proc[i].completion - pList->Proc[i].arrival;
    }
}

void findWaitingTime(struct ProcList *pList) {
    for (int i = 0 ; i < pList->processCount ; i++) {
        pList->Proc[i].waiting = pList->Proc[i].turnaround - pList->Proc[i].burst;
    }
}

void findAvgTime(struct ProcList *pList) {
    float totalTurnaround = 0, totalWaiting = 0;
    for (int i = 0 ; i < pList->processCount ; i++) {
        totalTurnaround += pList->Proc[i].turnaround;
        totalWaiting += pList->Proc[i].waiting;
    }
    pList->avgTurnaround = totalTurnaround / pList->processCount;
    pList->avgWaiting = totalWaiting / pList->processCount;
}

void printGanttChart(struct ProcList *pList) {
    int maxCompletionTime = 0;

    // Determine the maximum completion time to know how long the Gantt chart will be
    for (int i = 0 ; i < pList->processCount ; i++) {
        if (pList->Proc[i].completion > maxCompletionTime) {
            maxCompletionTime = pList->Proc[i].completion;
        }
    }

    printf("\nGantt Chart: (NOTE: May need to make Terminal Wider for longer Process Lists)\n");
    printf("Waiting: '----', Executing: '===='\n");

    // Print header row for time
    printf("Time:   ");
    for (int time = 0 ; time <= maxCompletionTime ; time++) {
        printf("%-4d ", time);
    }
    printf("\n");

    // Print process rows with waiting and execution periods
    for (int i = 0; i < pList->processCount; i++) { //BROKEN
        printf("P%d:    ", pList->Proc[i].id);
        for (int time = 0; time <= maxCompletionTime; time++) {
            if (time >= pList->Proc[i].arrival && time < pList->Proc[i].completion) {
                // If the process is in waiting period, print "----"
                if (time < pList->Proc[i].arrival + pList->Proc[i].waiting) {
                    printf("---- ");
                } else {
                    // Otherwise, print "====" for execution time
                    printf("==== ");
                }
            } else {
                // Empty space for non-execution times
                printf("     ");
            }
        }
        printf(" (%d)\n", pList->Proc[i].completion);
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

int main() {
    struct ProcList procList;
    procList.processCount = 0;
    int quantum;

    printf("========= Preemptive Round Robin =========\n\n");
    printf("Enter number of processes: ");
    scanf("%d", &procList.processCount);

    if (procList.processCount < 1) {
        printf("\nThere are no processes to calculate for!\nProgram Terminated.");
    } else {
        // Input quantum time
        printf("Enter time quantum: ");
        scanf("%d", &quantum);

        // Input process data
        for (int i = 0; i < procList.processCount; i++) {
            procList.Proc[i].id = i + 1;
            printf("\nEnter arrival time for process %d: ", procList.Proc[i].id);
            scanf("%d", &procList.Proc[i].arrival);
            printf("Enter burst time for process %d: ", procList.Proc[i].id);
            scanf("%d", &procList.Proc[i].burst);
            procList.Proc[i].remaining = procList.Proc[i].burst; // Set remaining burst time
            procList.Proc[i].completion = 0; // Initialize completion time
        }

        // Run the round robin scheduling
        roundRobin(&procList, quantum);

        // Calculate turnaround and waiting times
        findTurnaroundTime(&procList);
        findWaitingTime(&procList);
        findAvgTime(&procList);

        // Print the results
        printGanttChart(&procList);
    }

    return 0;
}