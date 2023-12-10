#include "process_generator.h"
#include <time.h>
#include <string.h>

void generateParameterizedProcesses(struct Process processes[], int n,
                                    int minArrival, int maxArrival,
                                    int minExecution, int maxExecution,
                                    int minPriority, int maxPriority) {
    for (int i = 0; i < n; ++i) {
        sprintf(processes[i].nom, "P%d", i + 1);
        processes[i].dateArrivee = rand() % (maxArrival - minArrival + 1) + minArrival;
        processes[i].dureeExecution = rand() % (maxExecution - minExecution + 1) + minExecution;
        processes[i].priorite = rand() % (maxPriority - minPriority + 1) + minPriority;
    }
}

void saveToFile(struct Process processes[], int n, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Process | Arrival Time | Execution Time | Priority |\n");
    fprintf(file, "__________________________________________\n");
    for (int i = 0; i < n; ++i) {
        fprintf(file, "%7s | %13d | %15d | %8d\n", processes[i].nom, processes[i].dateArrivee,
                processes[i].dureeExecution, processes[i].priorite);
    fprintf(file, "__________________________________________\n");
    }

    fclose(file);
}



void readResultsFromFile(const char *filename, struct Process *processes, size_t *numProcesses) {

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }


    char line[256];
    fgets(line, sizeof(line), file); 
    fgets(line, sizeof(line), file); 
    
    while (fgets(line, sizeof(line), file) != NULL && *numProcesses < 100) {

        int result = sscanf(line, " %99[^|]| %d | %d | %d",
                            processes[*numProcesses].nom, &processes[*numProcesses].dateArrivee,
                            &processes[*numProcesses].dureeExecution, &processes[*numProcesses].priorite);

        if (result == 4) {

            (*numProcesses)++;
        } else {
            fprintf(stderr, "Error parsing line: %s", line);
        }
    }

    fclose(file);
}


