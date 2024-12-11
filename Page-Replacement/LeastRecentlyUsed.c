#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 10
#define MAX_PAGES 30

void displayGanttChart(int pages[], int n) {
    printf("\nGantt Chart:\n");
    for (int i = 0; i < n; i++) {
        printf("| %d ", pages[i]);
    }
    printf("|\n");
}


void printProcessTable(int frames[], int frameCount) {
    // printf("\nProcess Table:\n");
    for (int i = 0; i < frameCount; i++) {
        if (frames[i] == -1) {
            printf("|   ");
        } else {
            printf("| %d ", frames[i]);
        }
    }
    printf("|\n");
}

int main() {
    int frames[MAX_FRAMES], pages[MAX_PAGES], time[MAX_FRAMES];
    int n, f, pageFaults = 0, pageHits = 0;
    float hitRatio;


    printf("Enter number of frames: ");
    scanf("%d", &f);

    printf("Enter number of pages: ");
    scanf("%d", &n);

    

    printf("Enter the reference string: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }

    for (int i = 0; i < f; i++) {
        frames[i] = -1;
        time[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int flag1 = 0, flag2 = 0;

        for (int j = 0; j < f; j++) {
            if (frames[j] == pages[i]) {
                pageHits++;
                time[j] = i + 1;
                flag1 = flag2 = 1;
                break;
            }
        }

        if (flag1 == 0) {
            for (int j = 0; j < f; j++) {
                if (frames[j] == -1) {
                    pageFaults++;
                    frames[j] = pages[i];
                    time[j] = i + 1;
                    flag2 = 1;
                    break;
                }
            }
        }

        if (flag2 == 0) {
            int pos = 0, min = time[0];
            for (int j = 1; j < f; j++) {
                if (time[j] < min) {
                    min = time[j];
                    pos = j;
                }
            }
            frames[pos] = pages[i];
            time[pos] = i + 1;
            pageFaults++;
        }

        // printf("\nProcess Table after inserting page %d:\n", pages[i]);
        for (int j = 0; j < f; j++) {
            if (frames[j] != -1) {
                printf("%d ", frames[j]);
            } else {
                printf("- ");
            }
        }
        printf("\n");
    }

    hitRatio = (float)pageHits / n;

    printf("\nTotal Page Hits: %d\n", pageHits);
    printf("Total Page Faults: %d\n", pageFaults);
    printf("Hit Ratio: %.2f\n", hitRatio);
    printf("Fault Ratio: %.2f\n", 1 - hitRatio);

    displayGanttChart(pages, n);

    return 0;
}