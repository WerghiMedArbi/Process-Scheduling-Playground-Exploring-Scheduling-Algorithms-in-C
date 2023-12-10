# Process Scheduling Playground: Exploring Scheduling Algorithms in C
## Short description :

Explore process scheduling in operating systems with C. This repository showcases scheduling algorithms (such as FIFO, SRT..). Run the code, observe results, and deepen your understanding of process management. Perfect for OS enthusiasts and developers. Happy coding!



### Topic Introduction :

The present document outlines the detailed specifications for the development project of a multitasking scheduling algorithm simulation system. The primary goal of this project is to design and implement a simulation environment capable of generating random tasks and applying various multitasking scheduling algorithms to assess their performance on a Linux operating system. The programming language used is C.



### Goals :

The project aims to provide a flexible and extensible simulation tool for studying and comparing the performance of multitasking scheduling algorithms. The main objectives include:


- Generating random tasks with various characteristics (execution time, priorities, arrival time).
- Implementing multiple multitasking scheduling algorithms.
- Allowing the configuration of simulation parameters, including the number of tasks, arrival intervals, scheduling parameters, etc.
- Collecting data on task execution and algorithm performance metrics.
- Visualizing simulation results through graphs and statistics.



## Installation guide:

Let’s start our mystical installation guide process!
### External libraries:

First, to interact with our graphic interface you need to install GTK library and to compile our code we need
the gcc compiler

`` sudo apt install libgtk-3-dev ``

`` sudo apt-get install gcc ``

### Restricting the program:
Once the program folder is in place, make it exclusive for admin wizards by limiting execution and
modification privileges.
Here’s an example: (path: ``/home/someone/Desktop/SEA``)

`` sudo chmod 744 /home/medarbiwerghi/Desktop/SEA ``


### Launching the program:

To fire up the application,we’ll create the executables by running a simple
makefile using the command

`` make -B ``

`` ./start ``

Now let’s delete all the object files and executables by using the clean instruction.

`` make clean ``


## TODO:

- Add diversified Scheduling Algorithms: (such as LIFO, MULTILEVEL Q,dynamic Priority…)
- Make an interface Upgrade with Detailed Statistics: (include detailed statistics on process execution, resource utilization, and algorithm efficiency)




# For more details:
check our report named " Documentation "
