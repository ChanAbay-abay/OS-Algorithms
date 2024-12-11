#include <stdio.h>

#define MAX_FRAMES 10
#define MAX_PAGES 30

void printGanttChart(int pages[], int n) {
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
    int frames[MAX_FRAMES], pages[MAX_PAGES];
    int frameCount, pageCount, pageHits;
    int pageFaults = 0, currentFrame = 0;
    pageHits = 0;

    printf("Enter number of frames: ");
    scanf("%d", &frameCount);

    printf("Enter number of pages: ");
    scanf("%d", &pageCount);

    printf("Enter page reference string: ");
    for (int i = 0; i < pageCount; i++) {
        scanf("%d", &pages[i]);
    }

    for (int i = 0; i < frameCount; i++) {
        frames[i] = -1;
    }

    for (int i = 0; i < pageCount; i++) {
        int page = pages[i];
        int found = 0;

        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == page) {
                found = 1;
                pageHits++;
                break;
            }
        }

        if (!found) {
            frames[currentFrame] = page;
            currentFrame = (currentFrame + 1) % frameCount;
            pageFaults++;
        }

        printProcessTable(frames, frameCount);
    }

    printGanttChart(pages, pageCount);

    printf("\nTotal Page Faults: %d\n", pageFaults);
    printf("Total Page Hits: %d\n", pageHits);

    float hitRatio = (float)pageHits / pageCount;
    float faultRatio = (float)pageFaults / pageCount;

    printf("Hit Ratio: %.2f\n", hitRatio);
    printf("Fault Ratio: %.2f\n", faultRatio);
    return 0;
}