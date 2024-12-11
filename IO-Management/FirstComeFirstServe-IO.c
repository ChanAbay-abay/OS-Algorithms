#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int current_position;
    int *requests;
    int num_requests;
    int track_size;
    int seek_rate;
} DiskArm;

// Function to calculate total head movement
int calculateTotalHeadMovement(DiskArm *disk) {
    int total_head_movement = 0;
    int previous_position = disk->current_position;

    printf("\nOrder of requests served: ");
    for (int i = 0; i < disk->num_requests; i++) {
        printf("%d ", disk->requests[i]);
        total_head_movement += abs(previous_position - disk->requests[i]);
        previous_position = disk->requests[i];
    }

    return total_head_movement;
}

// Function to process track requests in FCFS order
void processRequests(DiskArm *disk) {
    int total_head_movement = calculateTotalHeadMovement(disk);
    
    // Calculate seek time
    float seek_time = (float)total_head_movement * disk->seek_rate;
    
    printf("\nTotal head movement: %d units", total_head_movement);
    printf("\nSeek time: %.2f units of time\n", seek_time);
}

int main() {
    DiskArm disk;
    
    // Get user input for track size and current position
    printf("Enter the track size: ");
    scanf("%d", &disk.track_size);

    printf("Enter the current position of the disk arm: ");
    scanf("%d", &disk.current_position);

    // Get the number of requests
    printf("Enter the number of requests: ");
    scanf("%d", &disk.num_requests);

    // Dynamically allocate memory for track requests
    disk.requests = (int*)malloc(disk.num_requests * sizeof(int));

    // Get track requests from the user
    printf("Enter the track requests: ");
    for (int i = 0; i < disk.num_requests; i++) {
        scanf("%d", &disk.requests[i]);
    }

    // Get seek rate
    printf("Enter the seek rate: ");
    scanf("%d", &disk.seek_rate);

    // Process the requests and calculate total head movement and seek time
    processRequests(&disk);

    // Free allocated memory for requests
    free(disk.requests);

    return 0;
}
