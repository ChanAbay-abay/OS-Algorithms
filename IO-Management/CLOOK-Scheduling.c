#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_REQUESTS 100
#define MAX_MOVEMENT_DETAILS 200

typedef struct {
    int position;              // Current position of the disk arm
    int prev_position;         // Previous position of the disk arm
    int total_tracks;          // Total number of tracks on the disk
    int requests[MAX_REQUESTS]; // List of track requests
    int num_requests;          // Number of requests
    int movement;              // Total head movement
    char movement_details[MAX_MOVEMENT_DETAILS][100]; // Detailed movement calculations
    int movement_count;        // Counter for movement details
    int seek_rate;             // Seek rate
    int alpha;                 // Alpha value for seek time calculation
} DiskArm;

// Function to compare two integers (used for sorting)
int cmpfunc(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Function to add I/O requests to the disk arm
void add_requests(DiskArm *disk, int *requests, int num_requests) {
    for (int i = 0; i < num_requests; i++) {
        disk->requests[i] = requests[i];
    }
    disk->num_requests = num_requests;
}

// Function to process the requests using the C-SCAN algorithm
void process_requests(DiskArm *disk) {
    int right[MAX_REQUESTS];
    int left[MAX_REQUESTS];
    int right_count = 0, left_count = 0;

    // Separate the requests into two groups: right (>= current position) and left (< current position)
    for (int i = 0; i < disk->num_requests; i++) {
        if (disk->requests[i] >= disk->position) {
            right[right_count++] = disk->requests[i];
        } else {
            left[left_count++] = disk->requests[i];
        }
    }

    // Sort the right and left requests
    qsort(right, right_count, sizeof(int), cmpfunc);
    qsort(left, left_count, sizeof(int), cmpfunc);

    // Serve requests to the right first
    for (int i = 0; i < right_count; i++) {
        int movement = abs(disk->position - right[i]);
        disk->movement += movement;
        sprintf(disk->movement_details[disk->movement_count], "Move from %d → %d: |%d - %d| = %d units", disk->position, right[i], right[i], disk->position, movement);
        disk->movement_count++;
        disk->position = right[i];
    }

    // Move to the end of the disk and wrap around to the beginning
    if (right_count > 0) {
        int movement_to_end = abs(disk->position - (disk->total_tracks - 1));
        disk->movement += movement_to_end;
        sprintf(disk->movement_details[disk->movement_count], "Move from %d → %d (end): |%d - %d| = %d units", disk->position, disk->total_tracks - 1, disk->total_tracks - 1, disk->position, movement_to_end);
        disk->movement_count++;
        disk->position = 0;  // Wrap to the start

        // Add wrap to track 0 detail
        sprintf(disk->movement_details[disk->movement_count], "Wrap to track 0: No movement added");
        disk->movement_count++;

        for (int i = 0; i < left_count; i++) {
            int movement = abs(disk->position - left[i]);
            disk->movement += movement;
            sprintf(disk->movement_details[disk->movement_count], "Move from %d → %d: |%d - %d| = %d units", disk->position, left[i], left[i], disk->position, movement);
            disk->movement_count++;
            disk->position = left[i];
        }
    }

    // Calculate Seek Time considering alpha
    int seek_time = disk->movement * disk->seek_rate + disk->alpha;

    printf("\nTotal Seek Time (SK) with alpha %d: %d\n", disk->alpha, seek_time);
}

// Function to initialize a DiskArm
void init_disk_arm(DiskArm *disk, int initial_position, int total_tracks, int seek_rate, int alpha) {
    disk->position = initial_position;
    disk->prev_position = 0; // You can modify it if you need to track the previous position
    disk->total_tracks = total_tracks;
    disk->num_requests = 0;
    disk->movement = 0;
    disk->movement_count = 0;
    disk->seek_rate = seek_rate;
    disk->alpha = alpha;
}

int main() {
    DiskArm disk;
    int initial_position, total_tracks;

    // Get the total number of tracks and the initial position of the disk arm
    printf("Track Size: ");
    scanf("%d", &total_tracks);
    printf("Current Position ");
    scanf("%d", &initial_position);

    // Seek rate and alpha input
    int seek_rate, alpha;
    printf("Enter the seek rate: ");
    scanf("%d", &seek_rate);
    printf("Enter the alpha value: ");
    scanf("%d", &alpha);

    // Initialize the disk arm
    init_disk_arm(&disk, initial_position, total_tracks, seek_rate, alpha);

    // Input requests
    int num_requests;
    printf("Enter the number of track requests: ");
    scanf("%d", &num_requests);

    int requests[MAX_REQUESTS];
    printf("Enter the sequence of track requests (in track units, space-separated): ");
    for (int i = 0; i < num_requests; i++) {
        scanf("%d", &requests[i]);
    }

    // Add requests to the disk arm
    add_requests(&disk, requests, num_requests);

    // Process the requests
    process_requests(&disk);

    // Display results
    printf("\nOrder of track access (in track units): ");
    for (int i = 0; i < disk.movement_count; i++) {
        printf("%d ", disk.position); // Current position after each movement
    }
    printf("\n");

    printf("Detailed head movement calculations:\n");
    for (int i = 0; i < disk.movement_count; i++) {
        printf("%s\n", disk.movement_details[i]);
    }

    printf("Total head movement (in track units): %d units\n", disk.movement);

    return 0;
}
