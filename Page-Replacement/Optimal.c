#include <stdio.h>
#include <stdbool.h>

void printGanttChart(int pages[], int n) {
    printf("Gantt Chart:\n");
    for (int i = 0; i < n; i++) {
        printf("| %d ", pages[i]);
    }
    printf("|\n");
}

int findOptimal(int pages[], int frames[], int n, int frameCount, int index) {
    int farthest = index, frameIndex = -1;
    for (int i = 0; i < frameCount; i++) {
        int j;
        for (j = index; j < n; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    frameIndex = i;
                }
                break;
            }
        }
        if (j == n) {
            return i;
        }
    }
    return (frameIndex == -1) ? 0 : frameIndex;
}

void optimalPageReplacement(int pages[], int n, int frameCount) {
    int frames[frameCount];
    int pageFaults = 0, pageHits = 0;
    bool isFull = false;
    int index = 0;

    for (int i = 0; i < frameCount; i++) {
        frames[i] = -1;
    }

    printf("Process Table:\n");
    for (int i = 0; i < n; i++) {
        bool pageHit = false;
        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == pages[i]) {
                pageHit = true;
                pageHits++;
                break;
            }
        }

        if (!pageHit) {
            if (isFull) {
                int replaceIndex = findOptimal(pages, frames, n, frameCount, i + 1);
                frames[replaceIndex] = pages[i];
            } else {
                frames[index] = pages[i];
                index++;
                if (index == frameCount) {
                    isFull = true;
                }
            }
            pageFaults++;
        }

        for (int j = 0; j < frameCount; j++) {
            if (frames[j] != -1) {
                printf("%d ", frames[j]);
            } else {
                printf("- ");
            }
        }
        printf("\n");
    }

    printGanttChart(pages, n);

    printf("Page Hits: %d\n", pageHits);
    printf("Page Faults: %d\n", pageFaults);
    printf("Hit Ratio: %.2f\n", (float)pageHits / n);
    printf("Fault Ratio: %.2f\n", (float)pageFaults / n);
}

int main() {
    int frameCount, n;

    printf("Enter the number of frames: ");
    scanf("%d", &frameCount);

    printf("Enter the number of pages: ");
    scanf("%d", &n);

    int pages[n];
    printf("Enter the pages: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }

    optimalPageReplacement(pages, n, frameCount);

    return 0;
}