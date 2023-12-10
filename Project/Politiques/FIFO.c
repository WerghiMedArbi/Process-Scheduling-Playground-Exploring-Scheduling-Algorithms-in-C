#include <stdio.h>
#include <stdlib.h>

#include "../process_generator.h"

struct Process* fifoScheduling(struct Process processes[], int n) {


    qsort(processes, n, sizeof(struct Process), compareArrivalTime);
    

    for (int i = 0; i < n; ++i) {
        processes[i].tempsExecutionRestant = processes[i].dureeExecution;
        processes[i].waitingTime =0;
        processes[i].turnAroundTime =0;
        processes[i].endTime = -1;
        processes[i].startTime = processes[i].dateArrivee;
    }

      processes[0].waitingTime =0;
      processes[0].turnAroundTime = processes[0].dureeExecution;
    for (int i = 1; i < n; ++i) {
        processes[i].waitingTime = (processes[i].dateArrivee <= (processes[i - 1].turnAroundTime)) ? (processes[i - 1].turnAroundTime - processes[i].dateArrivee + processes[i-1].dateArrivee) : 0;
        processes[i].turnAroundTime = processes[i].waitingTime + processes[i].dureeExecution;
    }

    int s=0;
     for(int i = 0; i < n; ++i){
         s += processes[i].dureeExecution;
          }

    struct Process* results = malloc(s * sizeof(struct Process));
    if (results == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    int index=0;
     for (int i = 0; i < n; ++i) {
       for(int j = 0; j < processes[i].dureeExecution; ++j){
          results[index] = processes[i];
          index += 1;
          }
    }
    return results;
}

int compareArrivalTime(const void *a, const void *b) {
    return ((struct Process *)a)->dateArrivee - ((struct Process *)b)->dateArrivee;
}


