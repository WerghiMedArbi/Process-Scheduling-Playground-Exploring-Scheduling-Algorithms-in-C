#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "../process_generator.h"  

struct QueueNode {
    int processIndex;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};


void initializeQueue(struct Queue* q) {
    q->front = q->rear = NULL;
}


int isQueueEmpty(struct Queue* q) {
    return (q->front == NULL);
}


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


int dequeue(struct Queue* q) {
    if (isQueueEmpty(q)) {
        return -1; 
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

int comparePriority(const void *a, const void *b) {
    return ((struct Process *)b)->priorite - ((struct Process *)a)->priorite;
}
struct Process* multilevelScheduling(struct Process processes[], int n, int quantum) {
    

    qsort(processes, n, sizeof(struct Process), compareArrivalTime);

    int highestpriority = INT_MIN;
     for (int i = 0; i < n; ++i) {
        if(processes[i].priorite>highestpriority)
        highestpriority = processes[i].priorite;
        
    }

    struct Queue priorityQueues[highestpriority]; 
    for (int i = 0; i <= highestpriority; ++i) {
        initializeQueue(&priorityQueues[i]);
    }
    int s=0;
    for (int i = 0; i < n; ++i)
        s = s +  processes[i].dureeExecution;
    
    
    struct Process* results = malloc(s * sizeof(struct Process));
    if (results == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    

    for (int i = 0; i < n; ++i) {
        processes[i].tempsExecutionRestant = processes[i].dureeExecution;
        processes[i].waitingTime =0;
        processes[i].turnAroundTime =0;
        processes[i].endTime = -1;
        processes[i].startTime = processes[i].dateArrivee;
    }
  
    int ganttIndex = 0; 
    int currentTime = 0;
    int currentExecutingProcess = -1;
    int remainingQuantum = quantum;
    int consecutiveCount = 0;
    char lastExecutedProcess[50] = "";

    while (1) {

        for (int i = 0; i < n; ++i) {
            if (processes[i].dateArrivee == currentTime) {
                enqueue(&priorityQueues[processes[i].priorite], i);
            }
        }


        if (currentExecutingProcess != -1) {

            remainingQuantum--;


            if (processes[currentExecutingProcess].tempsExecutionRestant == 0) {
                processes[currentExecutingProcess].endTime= currentTime;
	  	processes[currentExecutingProcess].turnAroundTime= processes[currentExecutingProcess].endTime -processes[currentExecutingProcess].dateArrivee;
          	processes[currentExecutingProcess].waitingTime= processes[currentExecutingProcess].turnAroundTime - processes[currentExecutingProcess].dureeExecution;
          	currentExecutingProcess = -1;
                remainingQuantum = quantum;
                consecutiveCount = 0;
            } else if (remainingQuantum == 0) {


                enqueue(&priorityQueues[processes[currentExecutingProcess].priorite], currentExecutingProcess);
                remainingQuantum = quantum;
                processes[currentExecutingProcess].lastExecutedTime = currentTime;
                currentExecutingProcess = -1;
            }


            for (int i = highestpriority; i > processes[currentExecutingProcess].priorite; --i) {
                if (!isQueueEmpty(&priorityQueues[i])) {

                    enqueue(&priorityQueues[processes[currentExecutingProcess].priorite], currentExecutingProcess);
                    currentExecutingProcess = dequeue(&priorityQueues[i]);
                    remainingQuantum = quantum;
                    consecutiveCount = 0;
                    break;
                }
            }
        }


        if (currentExecutingProcess == -1) {
            for (int i = highestpriority ; i >= 0; --i) {
                if (!isQueueEmpty(&priorityQueues[i])) {

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


        if (currentExecutingProcess != -1) {
		processes[currentExecutingProcess].tempsExecutionRestant--;
		results[ganttIndex]= processes[currentExecutingProcess];
		ganttIndex ++;
        } 

        currentTime++;


        int allProcessesCompleted = 1;
        for (int i = 0; i < n; ++i) {
            if (processes[i].tempsExecutionRestant > 0) {
                allProcessesCompleted = 0;
                break;
            }
        }

        if (allProcessesCompleted) {
                processes[currentExecutingProcess].endTime= currentTime;
	  	processes[currentExecutingProcess].turnAroundTime= processes[currentExecutingProcess].endTime -processes[currentExecutingProcess].dateArrivee;
          	processes[currentExecutingProcess].waitingTime= processes[currentExecutingProcess].turnAroundTime - processes[currentExecutingProcess].dureeExecution;
            break;
        }
    }

    return results;
}
