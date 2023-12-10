#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "../process_generator.h"  

struct Process* roundRobinScheduling(struct Process processes[], int n, int quantum) {
    
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
     double currentTime = 0;
     int completedProcesses = 0;
     

    while (completedProcesses < n) {
    	double minArrival=INT_MAX;
        int flag=0; 
    	int index=-1;

        for (int i = 0; i < n; ++i) {
             double p = currentTime + 0.1; 
	     if(processes[i].startTime <= p && minArrival>processes[i].startTime && processes[i].tempsExecutionRestant>0){
		minArrival=processes[i].startTime;
		index=i;
		flag=1;
	     }                                                                        
        }
        
        if(flag==0){
          currentTime++;
          continue;
        }
        
        int executeTime;
        if(processes[index].tempsExecutionRestant < quantum){
          executeTime = processes[index].tempsExecutionRestant;
          currentTime += executeTime;
	  processes[index].tempsExecutionRestant=0;
	}
	else{
	  executeTime = quantum;
	  currentTime += executeTime;
	  processes[index].tempsExecutionRestant -= quantum;
	}
	
	 
	if( processes[index].tempsExecutionRestant >0){
	  processes[index].startTime = currentTime + 0.1;
	}

	 for(int j=ganttIndex;j< (ganttIndex+executeTime);j++)
           results[j]=processes[index];
         ganttIndex += executeTime;
	 

	if(processes[index].tempsExecutionRestant==0){
	  completedProcesses++;
	  processes[index].endTime= currentTime;
	  processes[index].turnAroundTime= processes[index].endTime -processes[index].dateArrivee;
          processes[index].waitingTime= processes[index].turnAroundTime - processes[index].dureeExecution;
	 
	}
  
    }
    return results;
}
