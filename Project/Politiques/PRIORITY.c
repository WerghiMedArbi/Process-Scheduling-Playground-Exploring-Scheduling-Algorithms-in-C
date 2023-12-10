#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "../process_generator.h" 



struct Process* priorityScheduling(struct Process processes[], int n) {


    qsort(processes, n, sizeof(struct Process), compareArrivalTime);

    int s=0;
     for(int i = 0; i < n; ++i){
         s += processes[i].dureeExecution;
          }

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

    int currentTime = 0; 
    int completedProcesses = 0; 
    int ganttIndex = 0; 
    int highestPriority = INT_MIN;
    int highestPriorityIndex = -1;
    while (completedProcesses < n) {
        

           for (int i = 0; i < n; ++i) {
             if (processes[i].dateArrivee <= currentTime && processes[i].tempsExecutionRestant > 0) {
                if (processes[i].priorite > highestPriority) {
                    highestPriority = processes[i].priorite;
                    highestPriorityIndex = i;
                }
             }
            }
        if(highestPriorityIndex != -1)
          {

            results[ganttIndex]= processes[highestPriorityIndex];
            ganttIndex++;
            

            processes[highestPriorityIndex].tempsExecutionRestant--;
          


            if (processes[highestPriorityIndex].tempsExecutionRestant == 0) {

                completedProcesses++;
                processes[highestPriorityIndex].endTime= currentTime + 1;
                processes[highestPriorityIndex].turnAroundTime= processes[highestPriorityIndex].endTime -processes[highestPriorityIndex].dateArrivee;
                processes[highestPriorityIndex].waitingTime= processes[highestPriorityIndex].turnAroundTime - processes[highestPriorityIndex].dureeExecution;
                highestPriority = INT_MIN;
                highestPriorityIndex = -1;
            }  
         }
      currentTime++;
    }

    return results;
}

