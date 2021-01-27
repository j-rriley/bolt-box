# Project 1, Discrete Event Simulator 
### Jaime Riley


In this program, I have created a discrete event simulator that processes events in a similar way to that of an OS, allocating resources to specific programs and releasing the resource for use once a process finishes up with it. 

In order to generate events, the program runs based off of fifteen pre-established values, defined within the header of the program, that specify the specific ways that the simulator will execute, including the length of time, the properties of the components, and the probabilities for certain events to occur. The core of the implementation of this program is five main structures, being four FIFO queues and one priority queue. These have been implemented in C by deriving from a linked list code, with each function of such manipulated to serve the goal of this program in different ways, including sorting, defining time, and overall cohesion. 

The priority queue is the main structure of this program; when an event is removed from it, there is an event-handler that is designated to take care of the job that is released. I have assigned each of the potential jobs a keycode, ranging from 0-10, including the events: handleJobArrival, handleArrivalCPU, handleCPUExit, handleDisk1Exit, handleArrivalDisk1… etc. These functions all serve different but equally meaningful purposes. All the events that handle arrivals are responsible for calling a function, generateTime, which will be talked about a bit later. 
    
All of the events that handle exits are responsible for redirecting the file once it is done and opens up the component for another use. Because of this, some functions require many more arguments as others, with the need to reference different FIFO queues, the priority queue, and a specific event each time the function is run. For all of this to connect, I had to create an event type with several characteristics to accomplish the goals of keeping track of time, of the next event in both a FIFO queue and priority queue, the event type, and the number job it was. There are also several utility functions within this program to make certain tasks much easier.

These functions include createLogFile, which opens an new file with a name designated by a character array argument, generateProbability, which given a double of probability (either exit or network) will return an int of 0 or 1 to represent sending the job to the network or to the disks, generateTime, which given two bounds will produce a random number between them, addLogFile, which takes in an event and writes to the log file depending on which event key it is, updateStats, which allows for the different properties of programs to be stored through a data structure I made called “STATS”.  Each time this program is run, it creates two files--a log file that details which of these jobs were processed, detailing the specific event that occurred and the time at which it took place, all in order of occurrence, and a stat-file, which details the statistics of each FIFOqueue calling them each by a value, 1 being CPU, 2 being Disk 1, 3 being Disk 2, and 4 being Network.

In order to get this program to execute properly, I had to use an ample amount of if-statements, else-statements, integer values of 0 and 1 to serve as a boolean, and while loops. Due to this, it executes slower than other programs, but still can run until any defined FIN_TIME. I mentioned most of my data structures already, a FIFOQueue, PQueue, STATS, and event, which all have different characteristic depending on their functions. My code is commented thoroughly throughout my program and has a clear work-flow. The logic goes as such:

1.	Start by initializing program with the seed for random numbers, the FIFOQueues and PQueues, the two files it is writing to, the STATS structure for each of the queues, and the events that are at INIT_TIME and FIN_TIME. 
2.	While the priority queue is not empty, pull the first event, determine which handler it needs, write it to the log, update the statistics of the stat file if applicable, and loop. 
3.	At FIN_TIME, an event will be called of key 10 that with handle exiting the system by setting the first node of the priority queue as null.
4.	Call the add log once again for this event, finish the stats by calling a program that will finalize the required values. 
5.	The simulation will exit and the program will be over. 


    
