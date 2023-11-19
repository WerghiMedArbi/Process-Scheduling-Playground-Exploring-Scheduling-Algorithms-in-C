
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Structure to represent a process
struct Process {
    char nom[50];
    int dateArrivee;
    int dureeExecution;
    int priorite;
};

// Function to generate random processes
void generateProcesses(struct Process processes[], int n) {
    for (int i = 0; i < n; ++i) {
        sprintf(processes[i].nom, "P%d", i + 1);
        processes[i].dateArrivee = rand() % 10;         // Random arrival time (0 to 9)
        processes[i].dureeExecution = rand() % 10 + 1;  // Random execution time (1 to 10)
        processes[i].priorite = rand() % 5 + 1;          // Random priority (1 to 5)
    }
}

// Function to print processes in a table format
void printTable(struct Process processes[], int n) {
    printf("Process | Arrival Time | Execution Time | Priority\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < n; ++i) {
        printf("%7s | %13d | %15d | %8d\n", processes[i].nom, processes[i].dateArrivee,
               processes[i].dureeExecution, processes[i].priorite);
    }
}

// Function to save processes to a file
void saveToFile(struct Process processes[], int n, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Process | Arrival Time | Execution Time | Priority |\n");
    fprintf(file, "-----------------------------------------------------\n");
    for (int i = 0; i < n; ++i) {
        fprintf(file, "%7s | %13d | %15d | %8d\n", processes[i].nom, processes[i].dateArrivee,
                processes[i].dureeExecution, processes[i].priorite);
    }

    fclose(file);
}

int main() {
    srand(time(NULL));

    int numProcesses;
    printf("Enter the number of processes: ");
    scanf("%d", &numProcesses);

    // Dynamically allocate memory for the processes
    struct Process *processes = (struct Process *)malloc(numProcesses * sizeof(struct Process));
    if (processes == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Generate random processes
    generateProcesses(processes, numProcesses);

    // Print the generated table
    printTable(processes, numProcesses);

    // Ask the user for the filename to save the results
    char filename[100];
    printf("\nEnter the filename to save the results: ");
    scanf("%s", filename);

    // Save processes to the specified file
    saveToFile(processes, numProcesses, filename);

    // Free allocated memory
    free(processes);

    return 0;
}

