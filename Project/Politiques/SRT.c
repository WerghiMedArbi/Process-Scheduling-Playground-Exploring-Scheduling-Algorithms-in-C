#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "../process_generator.h"  

struct Process* srtfScheduling(struct Process processes[], int n) {



    qsort(processes, n, sizeof(struct Process), compareArrivalTime);

    int s=0;
    for (int i = 0; i < n; ++i)
        s = s +  processes[i].dureeExecution;
    

    struct Process* results = malloc(100 * sizeof(struct Process));
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
    int shortestTime = INT_MAX;
    int shortestIndex = -1;
    while (completedProcesses < n) {
        

           for (int i = 0; i < n; ++i) {
             if (processes[i].dateArrivee <= currentTime && processes[i].tempsExecutionRestant > 0) {
                if (processes[i].tempsExecutionRestant < shortestTime) {
                    shortestTime = processes[i].tempsExecutionRestant;
                    shortestIndex = i;
                }
             }
            }
        if(shortestIndex != -1)
     	{

            
            results[ganttIndex]= processes[shortestIndex];
            ganttIndex++;
            

            processes[shortestIndex].tempsExecutionRestant--;
            

            shortestTime = processes[shortestIndex].tempsExecutionRestant;


            if (processes[shortestIndex].tempsExecutionRestant == 0) {

                completedProcesses++;
                processes[shortestIndex].endTime= currentTime + 1;
                processes[shortestIndex].turnAroundTime= processes[shortestIndex].endTime -processes[shortestIndex].dateArrivee;
                processes[shortestIndex].waitingTime= processes[shortestIndex].turnAroundTime - processes[shortestIndex].dureeExecution;
                shortestTime = INT_MAX;
                shortestIndex = -1;
            }
       }
            currentTime++;
     
    }

    return results;
}

