#include <stdio.h>
#include <stdlib.h>

struct Process {
    char nom[50];
    int dateArrivee;
    int dureeExecution;
    int priorite;
    int remainingTime; // Remaining execution time for the process
};

// Function to load processes from a file
void loadFromFile(struct Process processes[], int *n, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Skip the header lines
    char buffer[256];
    fgets(buffer, sizeof(buffer), file);
    fgets(buffer, sizeof(buffer), file);

    // Read processes from the file
    int i = 0;
    while (fscanf(file, "%s | %d | %d | %d", processes[i].nom, &processes[i].dateArrivee,
                  &processes[i].dureeExecution, &processes[i].priorite) == 4) {
        processes[i].remainingTime = processes[i].dureeExecution; // Initialize remaining time
        i++;
    }

    *n = i; // Update the number of processes

    fclose(file);
}

// Comparison function for sorting processes based on arrival time
int compareArrivalTime(const void *a, const void *b) {
    return ((struct Process *)a)->dateArrivee - ((struct Process *)b)->dateArrivee;
}

// Round-Robin scheduling algorithm
void roundRobinScheduling(struct Process processes[], int n, int quantum) {
    // Sort processes based on arrival time
    qsort(processes, n, sizeof(struct Process), compareArrivalTime);

    int currentTime = 0;
    int remainingProcesses = n;

    printf("Round-Robin Scheduling Order:\n");

    // Continue scheduling until all processes are executed
    while (remainingProcesses > 0) {
        for (int i = 0; i < n; ++i) {
            if (processes[i].remainingTime > 0 && processes[i].dateArrivee <= currentTime) {
                // Execute the process for the quantum or its remaining time, whichever is smaller
                int executeTime = (processes[i].remainingTime < quantum) ? processes[i].remainingTime : quantum;

                // Update remaining time for the process
                processes[i].remainingTime -= executeTime;

                // Update current time
                currentTime += executeTime;

                // Print the process name
                printf("%s ", processes[i].nom);

                // If the process is completed, decrement the remainingProcesses count
                if (processes[i].remainingTime == 0) {
                    remainingProcesses--;
                }
            }
        }
    }
    printf("\n");
}

int main() {
    int numProcesses;
    printf("Enter the filename to load processes from: ");
    char filename[100];
    scanf("%s", filename);

    struct Process *processes = malloc(100 * sizeof(struct Process)); // Assuming a maximum of 100 processes
    if (processes == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    loadFromFile(processes, &numProcesses, filename);

    int quantum;
    printf("Enter the time quantum for Round-Robin scheduling: ");
    scanf("%d", &quantum);

    roundRobinScheduling(processes, numProcesses, quantum);

    free(processes);

    return 0;
}

