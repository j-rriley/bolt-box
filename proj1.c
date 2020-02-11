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
    struct Event* next; 
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
    temp->next = NULL; 
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

  
// The queue, front stores the front node of LL and rear stores the last node of LL 
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
            struct Event* temp = placement->next; 
            placement->next = event; 
            event->next = temp; 
        }
        //if the place checked is a bigger time than the inserted event, then insert where that event currently is
        if(placement->time > timeOfNew) {
            sorted = 0; 
            struct Event* temp = placement; 
            placement = event; 
            event->next = placement; 
        }
        //if placement is NULL, then you are at the rear and you can insert here
        else{            
            sorted = 0; 
            q->rear->next = event; 
            q->rear = event; 
        }
        placement = placement->next; 
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



//                                         EVENT HANDLERS 

//this will be called when the job removed from the priority queue has eventkey 0. what will we do once a job has arrived? it will set the current time to 0 
//(the time of the event we removed from the queue), determine the arrival time fro the next job to enter system and add to queue
//send the first job to the CPU
void handleJobArrival(FILE* logFile, struct PQueue* priority, struct Event* event) {
    addLog(logFile, event); 
    event->time = 0;

    //set up new job 
    int currentEventNum = event->eventNum;
    int currentTime = event->time;

    struct Event* new = newEvent(currentEventNum + 1); 
    new->time = currentTime+(determineTime(ARRIVE_MIN,ARRIVE_MAX));
    new->eventType = 0;
    addPQ(priority, new);
    event->eventType = 1; 
    addPQ(priority, event);
}





//this is called when the event removed from the priority queue is of key 1. 
//therefore this must determine- Will it exit the system? will it do disk I/O or use the network? 

void handleCPUExit(FILE* logFile, struct PQueue* priority, struct FIFOQueue* CPU, struct Event* event) {
    addLog(logFile, event);
    //if the CPU FIFOQueue is not empty, add job to FIFOQueue
    if(CPU->front != NULL) {
        add(CPU, event);
    }
    //if the CPU FIFOQueue is empty, have the CPU process the event by seeing where it goes next. 
    //will it exit the event handler? 
    int quitProb = determineProbability(QUIT_PROB);
    //if yes, do not add anything to priority queue
    if(quitProb == 0) {
        return; 
    }
    //if no, check if it will go to network or disk
        else 
        {
            int networkProb = determineProbability(NETWORK_PROB);
            if(networkProb == 0) {
                event->eventType = 4;
                addPQ(priority, event); 
            }
            else{
                event->eventType = 2; 
                addPQ(priority, event);
            }
        }
}


//If event key is 2 or 3, it will use this handler. this will check the availability of both disks 

void handleUseOfDisks(FILE* logFile, struct FIFOQueue* disk1, struct FIFOQueue* disk2, struct Event* event) {

}

//Add to the network FIFOQueue

void handleUseOfNetwork(FILE* logFile, struct FIFOQueue* network, struct Event* event) {

}

void handleEndSimulation(FILE* logFile, struct PQueue* priority) {
   
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

    
}
