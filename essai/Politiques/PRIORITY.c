#include <stdio.h>
#include <stdlib.h>

struct Process {
    char nom[50];
    int dateArrivee;
    int dureeExecution;
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
        i++;
    }

    *n = i; // Update the number of processes

    fclose(file);
}

// Comparison function for sorting processes based on priority and arrival time
int comparePriorityAndArrival(const void *a, const void *b) {
    struct Process *processA = (struct Process *)a;
    struct Process *processB = (struct Process *)b;

    if (processA->priorite != processB->priorite) {
        return processB->priorite - processA->priorite; // Higher priority comes first
    } else {
        return processA->dateArrivee - processB->dateArrivee; // If priority is the same, use arrival time
    }
}

// Function to implement Priority scheduling algorithm
void priorityScheduling(struct Process processes[], int n) {
    // Sort processes based on priority and arrival time
    qsort(processes, n, sizeof(struct Process), comparePriorityAndArrival);

    // Execute processes in the order of their priority and arrival time
    printf("Priority Scheduling Order:\n");
    for (int i = 0; i < n; ++i) {
        printf("%s ", processes[i].nom);
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

    priorityScheduling(processes, numProcesses);

    free(processes);

    return 0;
}

