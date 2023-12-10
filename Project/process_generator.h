#ifndef PROCESS_GENERATOR_H
#define PROCESS_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>

struct Process {
    char nom[50];
    int dateArrivee;
    int dureeExecution;
    int priorite;
    int tempsExecutionRestant;
    int waitingTime;
    int turnAroundTime;
    double startTime;
    int endTime;
    int lastExecutedTime;
    char color[50];
};


struct Process* fifoScheduling(struct Process processes[], int n);
struct Process* srtfScheduling(struct Process processes[], int n);
struct Process* priorityScheduling(struct Process processes[], int n);
struct Process* roundRobinScheduling(struct Process processes[], int n, int quantum);
struct Process* multilevelScheduling(struct Process processes[], int n, int quantum);
int compareArrivalTime(const void *a, const void *b);

void generateParameterizedProcesses(struct Process processes[], int n,
                                    int minArrival, int maxArrival,
                                    int minExecution, int maxExecution,
                                    int minPriority, int maxPriority);

void saveToFile(struct Process processes[], int n, const char *filename);


void readResultsFromFile(const char *filename, struct Process *processes, size_t *numProcesses);



#endif /* PROCESS_GENERATOR_H */

