#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Process {
    char nom[50];
    int dateArrivee;
    int dureeExecution;
    int priorite;
    int tempsExecutionRestant;
    int lastExecutedTime; // For round-robin
};

struct QueueNode {
    int processIndex;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};

// Function to initialize a queue
void initializeQueue(struct Queue* q) {
    q->front = q->rear = NULL;
}

// Function to check if the queue is empty
int isQueueEmpty(struct Queue* q) {
    return (q->front == NULL);
}

// Function to enqueue a process in the queue
void enqueue(struct Queue* q, int processIndex) {
    struct QueueNode* newNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    if (newNode == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    newNode->processIndex = processIndex;
    newNode->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }

    q->rear->next = newNode;
    q->rear = newNode;
}

// Function to dequeue a process from the queue
int dequeue(struct Queue* q) {
    if (isQueueEmpty(q)) {
        return -1; // Queue is empty
    }

    struct QueueNode* temp = q->front;
    int processIndex = temp->processIndex;
    q->front = temp->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    return processIndex;
}

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

// Comparison function for sorting processes based on arrival time
int compareArrivalTime(const void *a, const void *b) {
    return ((struct Process *)a)->dateArrivee - ((struct Process *)b)->dateArrivee;
}

// Comparison function for sorting processes based on priority
int comparePriority(const void *a, const void *b) {
    return ((struct Process *)b)->priorite - ((struct Process *)a)->priorite;
}

// Function to implement preemptive priority scheduling algorithm with round robin
void priorityRoundRobinScheduling(struct Process processes[], int n, int quantum) {
    // Sort processes based on arrival time
    qsort(processes, n, sizeof(struct Process), compareArrivalTime);

    // Initialize queues for each priority level
    struct Queue priorityQueues[6]; // Assuming priorities range from 1 to 5, with an extra queue for processes with priority 0
    for (int i = 0; i < 6; ++i) {
        initializeQueue(&priorityQueues[i]);
    }

    int currentTime = 0;
    int currentExecutingProcess = -1;
    int remainingQuantum = quantum;
    int consecutiveCount = 0;
    char lastExecutedProcess[50] = "";

    while (1) {
        // Enqueue processes that have arrived
        for (int i = 0; i < n; ++i) {
            if (processes[i].dateArrivee == currentTime) {
                enqueue(&priorityQueues[processes[i].priorite], i);
            }
        }

        // Check if there's a process currently executing
        if (currentExecutingProcess != -1) {
            // Decrement the remaining quantum
            remainingQuantum--;

            // Check if the process has completed its execution
            if (processes[currentExecutingProcess].tempsExecutionRestant == 0) {
                currentExecutingProcess = -1;
                remainingQuantum = quantum;
                consecutiveCount = 0;
            } else if (remainingQuantum == 0) {
                // Quantum time is finished for the current process
                // Enqueue the process back with its priority and reset the quantum
                enqueue(&priorityQueues[processes[currentExecutingProcess].priorite], currentExecutingProcess);
                remainingQuantum = quantum;
                processes[currentExecutingProcess].lastExecutedTime = currentTime;
                currentExecutingProcess = -1;
            }

            // Check if a higher priority process has arrived
            for (int i = 5; i > processes[currentExecutingProcess].priorite; --i) {
                if (!isQueueEmpty(&priorityQueues[i])) {
                    // Preempt the current process
                    enqueue(&priorityQueues[processes[currentExecutingProcess].priorite], currentExecutingProcess);
                    currentExecutingProcess = dequeue(&priorityQueues[i]);
                    remainingQuantum = quantum;
                    consecutiveCount = 0;
                    break;
                }
            }
        }

        // Check if there's a process waiting to be executed
        if (currentExecutingProcess == -1) {
            for (int i = 5; i >= 0; --i) {
                if (!isQueueEmpty(&priorityQueues[i])) {
                    // Dequeue the highest priority process
                    currentExecutingProcess = dequeue(&priorityQueues[i]);
                    remainingQuantum = quantum;
                    if (strcmp(processes[currentExecutingProcess].nom, lastExecutedProcess) == 0) {
                        consecutiveCount++;
                    } else {
                        consecutiveCount = 1;
                        strcpy(lastExecutedProcess, processes[currentExecutingProcess].nom);
                    }
                    break;
                }
            }
        }

        // Print the current process
        if (currentExecutingProcess != -1) {
            if (consecutiveCount > 1) {
                printf("%s(%d) ", processes[currentExecutingProcess].nom, consecutiveCount);
            } else {
                printf("%s ", processes[currentExecutingProcess].nom);
            }
            processes[currentExecutingProcess].tempsExecutionRestant--;
        } else {
            printf("Idle ");
        }

        // Increment the current time
        currentTime++;

        // Check if all processes have completed their execution
        int allProcessesCompleted = 1;
        for (int i = 0; i < n; ++i) {
            if (processes[i].tempsExecutionRestant > 0) {
                allProcessesCompleted = 0;
                break;
            }
        }

        if (allProcessesCompleted) {
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

    int quantum;
    printf("Enter the time quantum for Round Robin: ");
    scanf("%d", &quantum);

    priorityRoundRobinScheduling(processes, numProcesses, quantum);

    free(processes);

    return 0;
}

