#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
struct Process {
    char nom[50];
    int dateArrivee;
    int dureeExecution;
    int priorite; 
    int tempsExecutionRestant;
    int lastExecutedTime; 
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
        processes[i].tempsExecutionRestant = processes[i].dureeExecution; // Initialize remaining execution time
        processes[i].lastExecutedTime = -1; // Initialize last executed time for round-robin
        i++;
    }

    *n = i; // Update the number of processes

    fclose(file);
}

/// Comparison function for sorting processes based on arrival time
int compareArrivalTime(const void *a, const void *b) {
    return ((struct Process *)a)->dateArrivee - ((struct Process *)b)->dateArrivee;
}

// Comparison function for sorting processes based on shortest remaining execution time
int compareRemainingExecutionTime(const void *a, const void *b) {
    return ((struct Process *)a)->tempsExecutionRestant - ((struct Process *)b)->tempsExecutionRestant;
}

// Function to implement non-preemptive shortest remaining execution time scheduling algorithm
void nonPreemptivePriorityScheduling(struct Process processes[], int n) {
    // Sort processes based on arrival time
    qsort(processes, n, sizeof(struct Process), compareArrivalTime);

    int currentTime = 0;

    while (true) {
        int nextExecutingProcess = -1;
        int nextArrivalTime = INT_MAX;

        // Find the next process with remaining execution time, considering both arrival time and shortest remaining execution time
        for (int i = 0; i < n; ++i) {
            if (processes[i].tempsExecutionRestant > 0
                && processes[i].dateArrivee <= currentTime) {
                // Compare remaining execution times for processes with the same arrival time
                if (processes[i].tempsExecutionRestant < processes[nextExecutingProcess].tempsExecutionRestant
                || (processes[i].tempsExecutionRestant == processes[nextExecutingProcess].tempsExecutionRestant
                && processes[i].dateArrivee < processes[nextExecutingProcess].dateArrivee)) {
                    nextExecutingProcess = i;
                    nextArrivalTime = processes[i].dateArrivee;
                }
            }
        }

        if (nextExecutingProcess == -1) {
            // All processes have completed their execution
            break;
        }

        currentTime = nextArrivalTime;

        while (processes[nextExecutingProcess].tempsExecutionRestant > 0) {
            // Execute the process
            printf("%s ", processes[nextExecutingProcess].nom);

            // Decrement the remaining execution time
            processes[nextExecutingProcess].tempsExecutionRestant--;

            // Increment the current time
            currentTime++;
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

    nonPreemptivePriorityScheduling(processes, numProcesses);

    free(processes);

    return 0;
}


