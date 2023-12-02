#include <stdio.h>
#include <stdlib.h>

struct Process {
    char nom[50];
    int dateArrivee;
    int dureeExecution;
    int tempsExecutionRestant;
    int priorite;
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
        i++;
    }

    *n = i; // Update the number of processes

    fclose(file);
}

// Comparison function for sorting processes based on remaining execution time
int compareRemainingExecutionTime(const void *a, const void *b) {
    return ((struct Process *)a)->tempsExecutionRestant - ((struct Process *)b)->tempsExecutionRestant;
}
// Comparison function for sorting processes based on arrival time
int compareArrivalTime(const void *a, const void *b) {
    return ((struct Process *)a)->dateArrivee - ((struct Process *)b)->dateArrivee;
}

// Function to implement Preemptive Shortest Job First (SJF) scheduling algorithm
void sjfScheduling(struct Process processes[], int n) {
    int currentTime = 0;
    qsort(processes, n, sizeof(struct Process), compareArrivalTime);
    while (1) {
        // Sort processes based on remaining execution time
        qsort(processes, n, sizeof(struct Process), compareRemainingExecutionTime);

        int found = 0; // Flag to check if a process is found to execute
        for (int i = 0; i < n; ++i) {
            if (processes[i].dateArrivee <= currentTime && processes[i].tempsExecutionRestant > 0) {
                printf("%s ", processes[i].nom);
                processes[i].tempsExecutionRestant--;

                // If the process completes its execution, update the current time
                if (processes[i].tempsExecutionRestant == 0) {
                    currentTime++;
                }

                found = 1;
                break;
            }
        }

        // If no process is found to execute, increment the current time
        if (!found) {
            currentTime++;
        }

        // Check if all processes are completed
        int allCompleted = 1;
        for (int i = 0; i < n; ++i) {
            if (processes[i].tempsExecutionRestant > 0) {
                allCompleted = 0;
                break;
            }
        }

        if (allCompleted) {
            break;
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

    sjfScheduling(processes, numProcesses);

    free(processes);

    return 0;
}

