#include <stdlib.h>
#include <stdio.h> 
#include <time.h>
#include <string.h>


//                               DATA BASED OFF CONFIG FILE
#define SEED 2
#define INIT_TIME 0 
#define FIN_TIME 10000
#define ARRIVE_MIN 5
#define ARRIVE_MAX 20
#define QUIT_PROB .22
#define NETWORK_PROB .88
#define CPU_MIN 10
#define CPU_MAX 20
#define DISK1_MIN 12
#define DISK1_MAX 30
#define DISK2_MIN 11
#define DISK2_MAX 55
#define NETWORK_MIN 20
#define NETWORK_MAX 50


//READ CONFIG FILE

/*
void readConfigFile(char* name) {
    char *input;
    input = name;
    FILE *stream = fopen(input, "r");
    //if file is invalid
    if(stream == NULL) {
        printf("Cannot open config\n");
        exit(1);
    }
    
    size_t size = 0;
    ssize_t read; 
    char* line = NULL;
    
    //values we need
    char* seed = ("SEED");
    char* init_time = ("INIT_TIME");
    char* fin_time = ("FIN_TIME");
    char* arrive_min = ("ARRIVE_MIN");
    char* arrive_max = ("ARRIVE_MAX");
    char* quit_prob = ("QUIT_PROB");
    char* network_prob = ("NETWORK_PROB");
    char* cpu_min = ("CPU_MIN");
    char* cpu_max = ("CPU_MAX");
    char* disk1_min = ("DISK1_MIN");
    char* disk1_max = ("DISK1_MAX");
    char* disk2_min = ("DISK2_MIN");
    char* disk2_max = ("DISK2_MAX");
    char* network_min = ("NETWORK_MIN");
    char* network_max = ("NETWORK_MAX");

    while((read = (getline(&line, &size, stream))) != -1) {
        
        if(strstr(line, seed) != NULL) {
            sscanf(line, "%d", &SEED);
        }
        if(strstr(line, init_time) != NULL) {
            sscanf(line, "%d", &INIT_TIME);            
        }
        if(strstr(line, fin_time) != NULL) {
            sscanf(line, "%d", &FIN_TIME);            
        }
        if(strstr(line, arrive_min) != NULL) {
            sscanf(line, "%d", &ARRIVE_MIN);            
        }
        if(strstr(line, arrive_max) != NULL) {
            sscanf(line, "%d", &ARRIVE_MAX);           
        }
        if(strstr(line, quit_prob) != NULL) {
            sscanf(line, "%d", &QUIT_PROB);            
        }
        if(strstr(line, network_prob) != NULL) {
            sscanf(line, "%d", &NETWORK_PROB);            
        }
        if(strstr(line, cpu_min) != NULL) {
            sscanf(line, "%d", &CPU_MIN);            
        }
        if(strstr(line, cpu_max) != NULL) {
            sscanf(line, "%d", &CPU_MAX);            
        }
        if(strstr(line, disk1_min) != NULL) {
            sscanf(line, "%d", &DISK1_MIN);            
        }
        if(strstr(line, disk1_max) != NULL) {
             sscanf(line, "%d", &DISK1_MAX);           
        }
        if(strstr(line, disk2_min) != NULL) {
             sscanf(line, "%d", &DISK2_MIN);           
        }
        if(strstr(line, disk2_max) != NULL) {
            sscanf(line, "%d", &DISK2_MAX);            
        }
        if(strstr(line, network_min) != NULL) {
             sscanf(line, "%d", &NETWORK_MIN);           
        }
        if(strstr(line, network_max) != NULL) {
            sscanf(line, "%d", &NETWORK_MAX);            
        }
    }
    fclose(stream);
}
*/

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
    int lastProcessedTime; 
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
    q->lastProcessedTime = 0;
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
    q->rear->nextFIFO = event; 
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
    q->front = q->front->nextFIFO; 
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
    
    // If the priority queue is empty, then new event is front and rear 
    if((q->front == NULL) && (q->rear == NULL)){ 
        q->front = q->rear = event; 
        return; 
    } 
  
    // If the queue is not empty, then add the new event to the queue based off the time
    int sorted = 1; 
    int timeOfNew = event->time;  


    struct Event* placement = q->front;
    
    int loop = 0;  
    while(sorted == 1) {
        loop++; 
        //if the place checked is smaller than the event trying to be added... we cannot sort unless it is the rear, the next time is greater than us, or the last thing in the queue
        if(placement->time < timeOfNew) {
            if(q->rear == placement)  {
                placement->nextPQ = event;
                q->rear =event;
                sorted = 0;
                continue; 
            }
            if(placement->nextPQ->time > timeOfNew) {
                struct Event* temp = placement->nextPQ; 
                placement->nextPQ = event;
                event->nextPQ = temp;  
                sorted = 0; 
                return; 
            }
            sorted = 1; 
            placement = placement->nextPQ; 
            continue; 
        }

        //if the place check is the same as the event trying to be added... we can sort
        if(placement->time == timeOfNew) {
            //if we are not at the rear, there means there is an event after that we must point to 
            if(q->rear != placement) {
                struct Event* temp = placement->nextPQ; 
                placement->nextPQ = event; 
                event->nextPQ = temp;
                sorted = 0; 
                return; 
            }
            //if we are at the rear, we can set ourselves as the rear after the placement
            else {
                placement->nextPQ = event; 
                q->rear = event; 
                sorted = 0;
                return; 
            }
        }

        //if the placement is bigger than the event trying to be added, we can sort if we are at the rear
        if(placement->time > timeOfNew) {
                struct Event* temp = placement; 
                q->front = event;
                event->nextPQ = temp;
                sorted = 0;
                return; 
            }
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
    q->front = q->front->nextPQ; 

  
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

    int eNum = event->eventNum;
    int eTime = event->time;

    if(event->eventType == 0) {
        fprintf(logFile, ("Job %d arrives at system at %d\n"), eNum, eTime);
        return;
    }
    if(event->eventType == 1) {
        fprintf(logFile, ("Job %d finishes at CPU at %d\n"), eNum, eTime);
        return;
    }
    if(event->eventType == 2) {
        fprintf(logFile, ("Job %d finishes at Disk 1 at %d\n"), eNum, eTime);
        return;
    }
    if(event->eventType == 3) {
        fprintf(logFile, ("Job %d finishes at Disk 2 at %d\n"), eNum, eTime);
        return;
    }
    if(event->eventType == 4) {
        fprintf(logFile, ("Job %d finishes at Network at %d\n"), eNum, eTime);
        return;
    }
    if(event->eventType == 5) {
        fprintf(logFile, ("Simulation is finished at %d\n"), eTime);
        return;
    }
    else {
        fprintf(logFile, ("Something went wrong, event type %d\n"), event->eventType);
        return;
    }
}

//determine time for arrival, CPU, disk 1, disk 2, or network
int determineTime(int min, int max) {
    int timeNeeded; 
    timeNeeded = (rand() % (max-min+1)) +min; 
    return timeNeeded; 
}


//determine quit, network, or I/O probability, where 0 is executes and 1 is do not execute
int determineProbability(double probType) {
    int boolean; 
    double place; 
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
    //getting stats of current job arrival
    int currentEventNum = (event->eventNum);
    int currentTime = (event->time);

    //creating the next job--->it will be job number of previous plus 1, it will have an arrival time that is the current time + a random value between arrive min and max. we will 
    //add it to the priority queue to be processed 
    struct Event* new = newEvent(currentEventNum+1); 
    new->time = currentTime + (determineTime(ARRIVE_MIN,ARRIVE_MAX));
    new->eventType = 0;
    addPQ(priority, new);

    //sending other job to CPU with amount of time it will be processed for
    event->eventType = 1; 
    event->time = (currentTime*2) - (CPU->lastProcessedTime) + determineTime(CPU_MIN,CPU_MAX);
    CPU->lastProcessedTime = event->time; 

    //if the CPU is freed up, immediately add event to Pqueue
    if(CPU->front == NULL) {
        CPU->lastProcessedTime = event->time;
        addPQ(priority, event);
        return; 
    }
    //if the CPU has events waiting, add it to the queue for processing
    else{
        add(CPU, event); 
        return;
    }
}





//this is called when the event removed from the priority queue is of key 1. 
//therefore this must determine- Will it exit the system? will it do disk I/O or use the network? 

void handleCPUExit(FILE* logFile, struct PQueue* priority, struct FIFOQueue* CPU, struct FIFOQueue* disk1, struct FIFOQueue* disk2, struct FIFOQueue* network, struct Event* event) {
    //time current event left CPU
    int currentTime = event->time; 
    
    //// -> DETERMINE WHERE IT GOES NEXT. first, check if the event is at the front of the queue. if so, remove it from the queue because it is being processed.
    struct Event* next = event->nextFIFO; 
    if(next != NULL) {
        next->time = currentTime + next->time; 
        CPU->lastProcessedTime = event->time;
        addPQ(priority, next);
    }
    
    
    //will it exit the event handler? 
    int quitProb = determineProbability(QUIT_PROB);
    
    //if yes, nothing else needs to be done here. add the next to the priority queue for processing because the CPU is free. 
    if(quitProb == 0) {
       return; 
    }

    //if no, check if it will go to network or disk
        else {
            int networkProb = determineProbability(NETWORK_PROB);
            if(networkProb == 0) {
                event->eventType = 4;
                event->time = determineTime(NETWORK_MIN, NETWORK_MAX);
                //if the network is freed up, immediately add new event to Pqueue
                     if(network->front == NULL) { 
                        event->time = event->time + network->lastProcessedTime;
                        network->lastProcessedTime = event->time; 
                        addPQ(priority, event);
                        return; 
                    }

                //if the network has events waiting, add it to the network queue for processing
                    else{
                        add(network, event); 
                        return; 
                    }    
            }
           
             //if not network, it will go to a disk. which disk? call disk function
            else{
                int diskChoice = checkWhichDisk(disk1, disk2);
                if(diskChoice == 1) {
                    event->eventType = 2;
                    event->time = determineTime(DISK1_MIN, DISK1_MAX);

                    //if disk1 is freed up, immediately add event to Pqueue
                     if(disk1->front == NULL) {
                        event->time = event->time + disk1->lastProcessedTime;
                        disk1->lastProcessedTime = event->time; 
                        addPQ(priority, event);
                        return; 
                    }
                    //if the disk1 has events waiting, add it to the disk1 queue for processing
                    else{
                        add(disk1, event); 
                        return; 
                    }
                }

                else{
                    event->eventType = 3;
                    event->time = determineTime(DISK2_MIN, DISK2_MAX);
                    //if disk2 is freed up, immediately add event to Pqueue (consider it processed)
                     if(disk2->front == NULL) {
                        event->time = event->time + disk2->lastProcessedTime;
                        disk2->lastProcessedTime = event->time; 
                        addPQ(priority, event);
                        return; 
                    }
                    //if the disk2 has events waiting, add it to the disk2 queue for processing
                    else{
                        add(disk2, event); 
                        return;
                    }
                }
            }
        }
}


//If event key is 2, it will use this handler. this will remove the event from the disk1 queue, send it back to the CPU for processing, and process the next job for disk1

void handleUseOfDisk1(FILE* logFile, struct PQueue* priority, struct FIFOQueue* CPU, struct FIFOQueue* disk1, struct Event* event) {
    //find next structure in Disk1 and add to priority queue

    //time current event left CPU
    int currentTime = event->time;

    struct Event* next = event->nextFIFO; 
    if(next != NULL) {
        next->time = currentTime + next->time; 
        disk1->lastProcessedTime = next->time; 
        addPQ(priority, next);
    }

    //check if event is at front of queue, if so remove and save for time purposes
    if(disk1->front == event) {
        removeQ(disk1);
    }
    

    //set up current event for CPU and remove from disk1
    event->eventType = 1; 
    event->time = determineTime(CPU_MIN, CPU_MAX);
    
    if(CPU->front == NULL) {
        event->time = event->time + CPU->lastProcessedTime;
        CPU->lastProcessedTime = event->time; 
        addPQ(priority, event);
        return; 
    }
    if(CPU->front != NULL) {
        add(CPU, event);
        return; 
    }
    
}


//If event key is 3, it will use this handler. this will remove the event from the disk2 queue and process the next

void handleUseOfDisk2(FILE* logFile, struct PQueue* priority, struct FIFOQueue* CPU, struct FIFOQueue* disk2, struct Event* event) {
    //time current event left CPU
    int currentTime = event->time;

    //find next structure in Disk22 and add to priority queue
    struct Event* next = event->nextFIFO; 

    if(next != NULL) {
        next->time = currentTime + next->time; 
        disk2->lastProcessedTime = next->time; 
        addPQ(priority, next);
    }
    //check if event is at front of queue, if so remove
    if(disk2->front == event) {
        removeQ(disk2);
    }
    

    //set up current event for CPU and remove from disk1
    event->eventType = 1; 
    event->time = determineTime(CPU_MIN, CPU_MAX);
    
    if(CPU->front == NULL) {
        event->time = event->time + CPU->lastProcessedTime;
        CPU->lastProcessedTime = event->time;
        addPQ(priority, event);
        return; 
    }
    if(CPU->front != NULL) {
        add(CPU, event);
        return; 
    }
}

//If event key is 4, it will use this handler. this will remove the event from the network queue and process the next

void handleUseOfNetwork(FILE* logFile, struct PQueue* priority, struct FIFOQueue* CPU, struct FIFOQueue* network, struct Event* event) {
    int currentTime = event->time; 
    //find next structure in Disk1 and add to priority queue
    struct Event* next = event->nextFIFO; 
    if(next != NULL) {
        next->time = currentTime + next->time; 
        network->lastProcessedTime = event->time;
        addPQ(priority, next);
    }

    //check if event is at front of queue, if so remove
    if(network->front == event) {
        removeQ(network);
    }
    
    //set up time for next
    event->eventType = 1; 
    event->time = determineTime(CPU_MIN, CPU_MAX);

    if(CPU->front == NULL) {
        event->time = event->time + CPU->lastProcessedTime;
        CPU->lastProcessedTime = event->time;
        addPQ(priority, event);
        return; 
    }
    if(CPU->front != NULL) {
        add(CPU, event);
        return; 
    }
}

void handleEndSimulation(struct PQueue* priority) {
    priority->front = NULL; 
}

void generateStatistics(struct Queue* q) {
    int averageSize;
    int maxSize;
    int utilization;
    int avgRespTime;
    int maxRespTime;
    int throughput;

    

}

//                                 IMPLEMENTATION OF DISCRETE EVENT SIMULATOR


int main(int argc, char **argv) {
    //readConfigFile("config.txt");
    time_t seed; 
    srand((unsigned) time(&seed));
    struct FIFOQueue* CPU = createQueue();
    struct FIFOQueue* disk1 = createQueue();
    struct FIFOQueue* disk2 = createQueue();
    struct FIFOQueue* network = createQueue();
    struct PQueue* priority = createPQueue(); 
    FILE* logFile = createLogFile("log-file");
    FILE* statistics = createLogFile("statistics");
    
    //initializing priority queue
    struct Event* event1 = newEvent(1);
    event1->eventType = 0; 
    event1->time = INIT_TIME; 
    addPQ(priority, event1);
    
    //endpoint for priority queue
    struct Event* eventLast = newEvent(0);
    eventLast->eventType = 5;
    eventLast->time = FIN_TIME; 
    addPQ(priority, eventLast);

    //event to be pulled
    struct Event* event;

    while(priority->front != NULL) {

        event = removePQ(priority);

        if(event->eventType == 0) {
            addLog(logFile, event);
            handleJobArrival(logFile, priority, CPU, event); 
            continue; 
        }
        
        if(event->eventType == 1) {
            addLog(logFile, event);
            handleCPUExit(logFile, priority, CPU, disk1, disk2, network, event);
            continue; 
        }

        if(event->eventType == 2) {
            addLog(logFile, event);
            handleUseOfDisk1(logFile, priority, CPU, disk1, event);
            continue; 
        }

        if(event->eventType == 3) {
            addLog(logFile, event);
            handleUseOfDisk2(logFile, priority, CPU, disk2, event);
            continue; 
        }

        if(event->eventType == 4) {
            addLog(logFile, event);
            handleUseOfNetwork(logFile, priority, CPU, network, event);
            continue; 
        }
        else {
            addLog(logFile, event);
            handleEndSimulation(priority); 
        }
    }

    
}
