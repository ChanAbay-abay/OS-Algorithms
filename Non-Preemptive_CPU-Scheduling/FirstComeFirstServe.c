#include <stdio.h>
#define MAX 10 // Maximum number of processes you can calculate for

struct Process{ // Each process
    int id;         // Process ID
    int waiting;    // Waiting time - waiting in the ready queue
    int arrival;    // Arrival time - arrives in ready queue
    int completion; // Completion time - process completion
    int burst;      // Burst time - time required by a process for CPU execution
    int turnaround; // Turnaround time - time difference between arrival & completion
};

struct ProcList{ // List of processes
    struct Process Proc[MAX];
    int processCount;
    float avgTurnaround; // Average turnaround time
    float avgWaiting;    // Average waiting time
};

void sortProcessesByArrival(struct ProcList *pList){ // Ensures FCFS by sorting according to Arrival Time
    for(int i = 0; i < pList->processCount - 1; i++){
        for(int j = 0; j < pList->processCount - i - 1; j++){
            if(pList->Proc[j].arrival > pList->Proc[j + 1].arrival){
                // Swap the processes
                struct Process temp = pList->Proc[j];
                pList->Proc[j] = pList->Proc[j + 1];
                pList->Proc[j + 1] = temp;
            }
        }
    }
}

void findCompletionTime(struct ProcList *pList){
    if(pList->processCount > 0){
        // First process completes
        pList->Proc[0].completion = pList->Proc[0].arrival + pList->Proc[0].burst; 
        for(int i = 1; i < pList->processCount; i++){
            // If process arrives after prev process has completed
            if(pList->Proc[i].arrival > pList->Proc[i - 1].completion){
                pList->Proc[i].completion = pList->Proc[i].arrival + pList->Proc[i].burst;
            }else{
                pList->Proc[i].completion = pList->Proc[i - 1].completion + pList->Proc[i].burst;
            }
        }
    }
}

void findTurnaroundTime(struct ProcList *pList) {
    if(pList->processCount > 0){
        for (int i = 0; i < pList->processCount; i++) {
            pList->Proc[i].turnaround = pList->Proc[i].completion - pList->Proc[i].arrival;
        }
    }
}

void findWaitingTime(struct ProcList *pList) {
    if(pList->processCount > 0){
        for (int i = 0; i < pList->processCount; i++) {
            pList->Proc[i].waiting = pList->Proc[i].turnaround - pList->Proc[i].burst;
        }
    }
}

void findAvgTime(struct ProcList *pList) {
    if(pList->processCount > 0){
        float totalTurnaround = 0, totalWaiting = 0;
        for(int i = 0; i < pList->processCount; i++) {
            totalTurnaround += pList->Proc[i].turnaround;
            totalWaiting += pList->Proc[i].waiting;
        }
        pList->avgTurnaround = totalTurnaround / pList->processCount; 
        pList->avgWaiting = totalWaiting / pList->processCount;       
    }
}

void printGanttChart(struct ProcList *pList) {
    if(pList->processCount > 0){
        printf("\nGantt Chart: (NOTE: May need to make Terminal Wider for longer Process Lists)\n");

        // Print header row for time
        printf("Time:   ");
        for(int time = 0; time <= pList->Proc[pList->processCount - 1].completion; time++){
            printf("%-4d ", time);
        }
        printf("\n");

        // Print process rows with bars
        for(int i = 0; i < pList->processCount; i++){
            //Print process ID (REMEMBER that its by order of input)
            printf("P%d:    ", pList->Proc[i].id);
            int startTime = (i == 0) ? pList->Proc[i].arrival : pList->Proc[i - 1].completion;
            for(int time = 0; time <= pList->Proc[pList->processCount - 1].completion; time++){
                if(time >= startTime && time < pList->Proc[i].completion){
                    printf("==== ");
                }else{
                    printf("     ");
                }
            }
            //Print completion time at the end
            printf(" (%d)", pList->Proc[i].completion);
            printf("\n");
        }


        //Display process summary table
        printf("\nProcess Summary Table:\n");
        printf("-------------------------------------------------------------\n");
        printf("| ID  | Arrival | Burst | Completion | Turnaround | Waiting |\n");
        printf("|-----|---------|-------|------------|------------|---------|\n");
        // printf("ID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");
        for(int i = 0; i < pList->processCount; i++){
        // printf("| %d |   %d    |  %d   |     %d     |     %d     |   %d    |\n");
            printf("| %2d  |   %2d    |  %2d   |     %2d     |     %2d     |   %2d    |\n",
                pList->Proc[i].id,
                pList->Proc[i].arrival,
                pList->Proc[i].burst,
                pList->Proc[i].completion,
                pList->Proc[i].turnaround,
                pList->Proc[i].waiting);
        }
        printf("-------------------------------------------------------------\n");

        //Display average times at the bottom of the Gantt chart
        printf("Avg Turnaround Time: %.2f\n", pList->avgTurnaround);
        printf("Avg WT: %.2f\n", pList->avgWaiting);
    }
}

int main() {
    struct ProcList procList;
    procList.processCount = 0;

    // Number of processes input
    printf("========= First Come First Serve =========\n\n");
    printf("Enter number of processes: ");
    scanf("%d", &procList.processCount);

    // Process details input
    if(procList.processCount < 1){
        printf("\nThere are no processes to calculate for!\nProgram Terminated.");
    }else{
        // Input for arrival and burst time of each process
        for(int i = 0; i < procList.processCount; i++){
            procList.Proc[i].id = i + 1; // Process ID starts at 1 & auto increments
            printf("\nEnter arrival time for process %d: ", procList.Proc[i].id);
            scanf("%d", &procList.Proc[i].arrival);
            printf("Enter burst time for process %d: ", procList.Proc[i].id);
            scanf("%d", &procList.Proc[i].burst);
        }

        // Maintain FCFS
        sortProcessesByArrival(&procList);

        // Calculating for each process
        findCompletionTime(&procList);
        findTurnaroundTime(&procList);
        findWaitingTime(&procList);
        findAvgTime(&procList);
        printGanttChart(&procList);
    }

    return 0;
}