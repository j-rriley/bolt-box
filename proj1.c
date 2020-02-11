#include <stdlib.h>
#include <stdio.h> 





//                               DATA BASED OFF CONFIG FILE
#define SEED
#define INIT_TIME 
#define FIN_TIME
#define ARRIVE_MIN
#define ARRIVE_MAX
#define QUIT_PROB
#define NETWORK_PROB
#define CPU_MIN
#define CPU_MAX
#define DISK1_MIN
#define DISK1_MAX
#define DISK2_MIN
#define DISK2_MAX
#define NETWORK_MIN
#define NETWORK_MAX

//An event structure

// A linked list (LL) node to store a queue entry 
struct Event { 
    int eventNum; 
    int time; 
    int eventType; 
    struct Event* nextPQ;
    struct Event* nextFIFO;  
}; 
  
// The FIFOQueue, front stores the front node of LL and rear stores the last node of LL 
struct FIFOQueue { 
    struct Event *front, *rear; 
}; 
  
// A utility function to create a new event based off its number. 
struct Event* newEvent(int number) 
{ 
    struct Event* temp = (struct Event*)malloc(sizeof(struct Event)); 
    temp->eventNum = number; 
    temp->nextPQ = NULL; 
    return temp; 
} 
  
// A utility function to create an empty FIFOQueue 
struct FIFOQueue* createQueue() 
{ 
    struct FIFOQueue* q = (struct FIFOQueue*)malloc(sizeof(struct FIFOQueue)); 
    q->front = q->rear = NULL; 
    return q; 
} 
  
// The function to add an event to a FIFOQueue 
void add(struct FIFOQueue* q, struct Event* event) 
{ 
    
    // If FIFOQueue is empty, then new node is front and rear both 
    if (q->rear == NULL) { 
        q->front = q->rear = event; 
        return; 
    } 
  
    // If it is not empty, add the new node at the end of FIFOQueue and change rear 
    q->rear->next = event; 
    q->rear = event; 
} 
  
// Function to remove a key from given FIFOQueue q 
struct Event* removeQ(struct FIFOQueue* q) 
{ 
    // If FIFOQueue is empty, return NULL. 
    if (q->front == NULL) {
        return NULL; 
    }
    // Store previous front and move front one node ahead 
    struct Event* temp = q->front; 
    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 
    if (q->front == NULL) {
        q->rear = NULL; 
    }
    return temp; 
} 







//                            PRIORITY QUEUE DATA STRUCTURE FOR EVENT HANDLING

  
// The priority queue, storing both the front and the rear
struct PQueue { 
    struct Event *front, *rear; 
}; 
  
// A utility function to create an empty priority queue 
struct PQueue* createPQueue() 
{ 
    struct PQueue* q = (struct PQueue*)malloc(sizeof(struct PQueue)); 
    q->front = q->rear = NULL; 
    return q; 
} 
  
// The function to add a new event to priority queue  
void addPQ(struct PQueue* q, struct Event* event) 
{ 
    
    // If queue is empty, then new node is front and rear both and time is 0 
    if (q->rear == NULL) { 
        q->front = q->rear = event; 
        return; 
    } 
  
    // If the queue is not empty, then add the new event to the queue based off the time
    int sorted = 0; 
    int timeOfNew = event->time;  
    struct Event* placement = q->front;
    while(sorted == 1) {
        //if the place checked is a smaller time than the inserted event, then do not insert
        if(placement->time < timeOfNew) {
            sorted = 1; 
        }
        //if the place checked is a the same time as the inserted event and the next event's time is greater than or equal to the inserted event, insert here
        if(placement->time = timeOfNew && placement->next->time >= timeOfNew) {
            sorted = 0; 
            struct Event* temp = placement->nextPQ; 
            placement->nextPQ = event; 
            event->nextPQ = temp; 
        }
        //if the place checked is a bigger time than the inserted event, then insert where that event currently is
        if(placement->time > timeOfNew) {
            sorted = 0; 
            struct Event* temp = placement; 
            placement = event; 
            event->nextPQ = placement; 
        }
        //if placement is NULL, then you are at the rear and you can insert here
        else{            
            sorted = 0; 
            q->rear->next = event; 
            q->rear = event; 
        }
        placement = placement->nextPQ; 
    }
    
} 
  
// Function to remove a key from given queue q 
struct Event* removePQ(struct PQueue* q) 
{ 
    // If queue is empty, return NULL. 
    if (q->front == NULL) {
        return NULL; 
    }

    // Store previous front and move front one node ahead 
    struct Event* temp = q->front; 
    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 
    if (q->front == NULL) {
        q->rear = NULL; 
    }

    return temp; 
} 







//                                       LOG FILE CREATION


FILE* createLogFile(char* name) {
    FILE *logFile = fopen(name, "w");
    if(logFile == NULL) {
        printf("Unable to create log file.\n");
        exit(1);
    }
    else {
        return logFile; 
    }
}

//print out occurrence

void addLog(FILE* logFile, struct Event* event) {
    char* eventPrint = "";
    int eNum = event->eventNum;
    int eTime = event->time;

    if(event->eventType == 0) {
        *eventPrint = ("Job %d arrives at system at %d\n", eNum, eTime);
    }
    if(event->eventType == 1) {
        *eventPrint = ("Job %d finishes at CPU at %d\n", eNum, eTime);
    }
    if(event->eventType == 2) {
        *eventPrint = ("Job %d finishes at Disk 1 at %d\n", eNum, eTime);
    }
    if(event->eventType == 3) {
        *eventPrint = ("Job %d finishes at Disk 2 at %d\n", eNum, eTime);
    }
    if(event->eventType == 4) {
        *eventPrint = ("Job %d finishes at Network at %d\n", eNum, eTime);
    }
    if(event->eventType == 5) {
        *eventPrint = ("Simulation is finished at %d\n", eTime);
    }
    else {
        eventPrint = ("Something went wrong\n");
    }
    fputs(eventPrint, logFile);
}

//determine time for arrival, CPU, disk 1, disk 2, or network
int determineTime(int min, int max) {
    int timeNeeded; 
    timeNeeded = (rand() % (max-min+1)) +min; 
    return timeNeeded; 
}


//determine quit, network, or I/O probability, where 0 is executes and 1 is do not execute
int determineProbability(int probType) {
    int boolean; 
    int place; 
    place = (rand() % 100);
    if(place >= (probType*100)) {
        boolean = 0; 
    }
    else {
        boolean = 1; 
    }
    return boolean; 
}

int checkWhichDisk(struct FIFOQueue* disk1, struct FIFOQueue* disk2) {
    int whichDisk = -1; 
    //if the disk 1 queue doesnt have components
    if(disk1->front == NULL) {
        whichDisk = 1; 
    }
    //if disk 2 queue doesnt have components
    if(disk2->front == NULL){
        whichDisk = 2; 
    }
    //if both have components, see which has less and decide
    else {
        int disk1Count = 1;
        int disk2Count = 1; 
        struct Event* placement1 = disk1->front->nextFIFO; 
        struct Event* placement2 = disk2->front->nextFIFO; 
        while(placement1 != NULL) {
            disk1Count++; 
            placement1 = placement1->nextFIFO; 
        } 
        while(placement2 != NULL) {
            disk2Count++; 
            placement2 = placement2->nextFIFO; 
        } 
        if(placement1 <= placement2) {
            whichDisk = 1; 
        }
        else{
            whichDisk = 2; 
        }
    }
    return whichDisk; 
}

//                                         EVENT HANDLERS 

//this will be called when the job removed from the priority queue has eventkey 0. what will we do once a job has arrived? it will set the current time to 0 
//(the time of the event we removed from the queue), determine the arrival time for the next job to enter system and add to queue
//send the first job to the CPU

void handleJobArrival(FILE* logFile, struct PQueue* priority, struct FIFOQueue* CPU, struct Event* event) {
    addLog(logFile, event); 

    //getting states for next job arrival
    int currentEventNum = event->eventNum;
    int currentTime = event->time;

    //creating the next job
    struct Event* new = newEvent(currentEventNum + 1); 
    new->time = currentTime+(determineTime(ARRIVE_MIN,ARRIVE_MAX));
    new->eventType = 0;
    addPQ(priority, new);

    //sending last job to CPU with amount of time it will be processed for
    event->eventType = 1; 
    event->time = (event->time) + determineTime(CPU_MIN, CPU_MAX);

    //if the CPU is freed up, immediately add event to Pqueue
    if(CPU->front == NULL) {
        addPQ(priority, event);
    }

    //if the CPU has events waiting, add it to the queue for processing
    else{
        add(CPU, event); 
    }
}





//this is called when the event removed from the priority queue is of key 1. 
//therefore this must determine- Will it exit the system? will it do disk I/O or use the network? 

void handleCPUExit(FILE* logFile, struct PQueue* priority, struct FIFOQueue* CPU, struct FIFOQueue* disk1, struct FIFOQueue* disk2, struct FIFOQueue* network, struct Event* event) {
    addLog(logFile, event);

    //// -> DETERMINE WHERE IT GOES NEXT

    //will it exit the event handler? 
    int quitProb = determineProbability(QUIT_PROB);
    
    //if yes, nothing else needs to be done here, so we will remove the event from the CPUqueue and add the next to the priority queue for processing
    if(quitProb == 0) {
        struct Event* next = event->nextFIFO; 
        if(next != NULL) {
           addPQ(priority, next);
        }
       removeQ(CPU);
    }

    //if no, check if it will go to network or disk
        else 
        {
            int networkProb = determineProbability(NETWORK_PROB);
            if(networkProb == 0) {
                event->eventType = 4;
                event->time = (event->time) + determineTime(NETWORK_MIN, NETWORK_MAX);
                removeQ(CPU);

                //if the network is freed up, immediately add new event to Pqueue and remove old from 
                     if(network->front == NULL) {
                        addPQ(priority, event);
                    }

                //if the network has events waiting, add it to the network queue for processing
                    else{
                        add(network, event); 
                    }    
            }
           
             //if not network, it will go to a disk. which disk? call disk function
            else{
                int diskChoice = checkWhichDisk(disk1, disk2);
                if(diskChoice == 1) {
                    event->eventType = 2;
                    event->time = (event->time) + determineTime(DISK1_MIN, DISK1_MAX);
                    removeQ(CPU);
                    //if disk1 is freed up, immediately add event to Pqueue
                     if(disk1->front == NULL) {
                        addPQ(priority, event);
                    }
                    //if the disk1 has events waiting, add it to the disk1 queue for processing
                    else{
                        add(disk1, event); 
                    }
                }
                else{
                    event->eventType = 3;
                    event->time = (event->time) + determineTime(DISK2_MIN, DISK2_MAX);
                    removeQ(CPU);
                    //if disk2 is freed up, immediately add event to Pqueue (consider it processed)
                     if(disk2->front == NULL) {
                        addPQ(priority, event);
                    }
                    //if the disk2 has events waiting, add it to the disk2 queue for processing
                    else{
                        add(disk2, event); 
                    }
                }
            }
        }
}


//If event key is 2, it will use this handler. this will remove the event from the disk1 queue, send it back to the CPU for processing, and process the next job for disk1

void handleUseOfDisk1(FILE* logFile, struct PQueue* priority, struct FIFOQueue* CPU, struct FIFOQueue* disk1, struct Event* event) {
   
    addLog(logFile, event);

    //find next structure in Disk1
    struct Event* next = event->nextFIFO; 

    //set up current event for CPU and remove from disk1
    event->eventType = 1; 
    event->time = (event->time) + determineTime(CPU_MIN, CPU_MAX);
    removeQ(disk1);
    add(CPU, event);

    //process the next event by the disk
    addPQ(priority, next);
    
}


//If event key is 3, it will use this handler. this will remove the event from the disk2 queue and process the next

void handleUseOfDisk2(FILE* logFile, struct PQueue* priority, struct FIFOQueue* CPU, struct FIFOQueue* disk2, struct Event* event) {
    
    addLog(logFile, event);

    //find next structure in Disk1
    struct Event* next = event->nextFIFO; 

    //set up current event for CPU and remove from disk1
    event->eventType = 1; 
    event->time = (event->time) + determineTime(CPU_MIN, CPU_MAX);
    removeQ(disk2);
    add(CPU, event);

    //process the next event by disk 2 if there is one
    if(next != NULL) {
    addPQ(priority, next);
    }
}

//If event key is 4, it will use this handler. this will remove the event from the network queue and process the next

void handleUseOfNetwork(FILE* logFile, struct PQueue* priority, struct FIFOQueue* CPU, struct FIFOQueue* network, struct Event* event) {

    addLog(logFile, event);

    //find next structure in network
    struct Event* next = event->nextFIFO; 

    //set up current event for CPU and remove from network
    event->eventType = 1; 
    event->time = (event->time) + determineTime(CPU_MIN, CPU_MAX);
    removeQ(network);
    add(CPU, event);

    //process the next event by the network if there is one
    if(next != NULL) {
    addPQ(priority, next);
    }
}

void handleEndSimulation(FILE* logFile, struct Event* endSim) {
    addLog(logFile, endSim);
}








//                                 IMPLEMENTATION OF DISCRETE EVENT SIMULATOR


int main(int argc, char **argv) {
    srand((int)SEED);
    struct FIFOQueue* CPU = createQueue();
    struct FIFOQueue* disk1 = createQueue();
    struct FIFOQueue* disk2 = createQueue();
    struct FIFOQueue* network = createQueue();
    struct PQueue* priority = createQueue(); 
    FILE* logFile = createLogFile("log-file");

    //initializing priority queue
    struct Event* event1 = newEvent(1);
    event1->eventType = 0; 
    event1->time = INIT_TIME; 
    addPQ(priority, event1);
    
    //endpoint for priority queue
    struct Event* eventLast = newEvent(0);
    eventLast->eventType = 5;
    eventLast->time = FIN_TIME; 

    //event to be pulled
    struct Event* event;

    while(priority->front != NULL) {
        
        event = removePQ(priority);

        if(event->eventType == 0) {
            handleJobArrival(logFile, priority, CPU, event);
        }
        
        if(event->eventType == 1) {
            handleCPUExit(logFile, priority, CPU, disk1, disk2, network, event);
        }

        if(event->eventType == 2) {
            handleUseOfDisk1(logFile, priority, CPU, disk1, event);
        }

        if(event->eventType == 3) {
            handleUseOfDisk2(logFile, priority, CPU, disk2, event);
        }

        if(event->eventType == 4) {
            handleUseOfNetwork(logFile, priority, CPU, network, event);
        }
        else {
            handleEndSimulation(); 
            break; 
        }
    }

    
}
