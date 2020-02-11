#include <stdlib.h>
#include <stdio.h> 





//                               DATA BASED OFF CONFIG FILE
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
  
// The queue, front stores the front node of LL and rear stores the last node of LL 
struct Queue { 
    struct Event *front, *rear; 
}; 
  
// A utility function to create a new linked list node. 
struct Event* newNode(int k) 
{ 
    struct Event* temp = (struct Event*)malloc(sizeof(struct Event)); 
    temp->eventNum = k; 
    temp->next = NULL; 
    return temp; 
} 
  
// A utility function to create an empty queue 
struct Queue* createQueue() 
{ 
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue)); 
    q->front = q->rear = NULL; 
    return q; 
} 
  
// The function to add a key k to q 
void add(struct Queue* q, int k) 
{ 
    // Create a new LL node 
    struct Event* temp = newNode(k); 
  
    // If queue is empty, then new node is front and rear both 
    if (q->rear == NULL) { 
        q->front = q->rear = temp; 
        return; 
    } 
  
    // Add the new node at the end of queue and change rear 
    q->rear->next = temp; 
    q->rear = temp; 
} 
  
// Function to remove a key from given queue q 
struct Event* remove(struct Queue* q) 
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







//                            PRIORITY QUEUE DATA STRUCTURE FOR EVENT HANDLING

  
// The queue, front stores the front node of LL and rear stores the last node of LL 
struct PQueue { 
    struct Event *front, *rear; 
}; 
  
  
// A utility function to create an empty priority queue 
struct PQueue* createPQueue() 
{ 
    struct Queue* q = (struct PQueue*)malloc(sizeof(struct PQueue)); 
    q->front = q->rear = NULL; 
    return q; 
} 
  
// The function to add a key k to q 
void addPQ(struct PQueue* q, int k) 
{ 
    // Create a new LL node 
    struct Event* temp = newNode(k); 
    
    // If queue is empty, then new node is front and rear both and time is 0 
    if (q->rear == NULL) { 
        q->front = q->rear = temp; 
        return; 
    } 
  
    // Add the new node at the end of queue and change rear 
    q->rear->next = temp; 
    q->rear = temp; 
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


FILE* createLogFile(char[] name) {
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
        eventPrint = ("Job %d arrives at system at %d\n", eNum, eTime);
    }
    if(event->eventType == 1) {
        eventPrint = ("Job %d finishes at CPU at %d\n", eNum, eTime);
    }
    if(event->eventType == 2) {
        eventPrint = ("Job %d finishes at Disk 1 at %d\n", eNum, eTime);
    }
    if(event->eventType == 3) {
        eventPrint = ("Job %d finishes at Disk 2 at %d\n", eNum, eTime);
    }
    if(event->eventType == 4) {
        eventPrint = ("Job %d finishes at Network at %d\n", eNum, eTime);
    }
    if(event->eventType == 5) {
        eventPrint = ("Simulation is finished at %d\n", eTime);
    }
    else {
        eventPrint = ("Something went wrong\n");
    }
    fputs(eventPrint, logFile);
}





//                                         EVENT HANDLERS 


struct Event* handleJobArrival(FILE* logFile, struct Queue* priority, struct Event* event) {
    addLog(logfile, event);
    //if the priority queue is not empty, add job to queue
    if(priority->front != NULL) {
        event->eventType = 0; 
        add(priority, event);
    }

    //if the priority queue is empty, handle use of CPU
    else {
        return event; 
    }
}




//Will it exit the system? will it do disk I/O or use the network? 

struct Event* handleCPUExit(FILE* logFile, struct Queue* CPU, struct Event* event) {
    addLog(logfile, event);

    //if the CPU queue is not empty, add job to queue
    if(CPU->front != NULL) {
        CPU.add(event);
    }

    //if the CPU queue is empty, have the CPU process the event.
    else {
        char[] event = ("Job %d finishes at CPU at %d\n", (event->eventNum), (event->time));
        fputs(event, logFile);
        handleCPUExit(logfile, CPU, event); 
    }

    return event; 
}


//If it will be read by a disk: add to the disk that is free for use

void handleUseOfDisks(FILE* logFile, struct Queue* disk1, struct Queue* disk2, struct Event* event) {

}

//Add to the network queue

void handleUseOfNetwork(FILE* logFile, struct Queue* network, struct Event* event) {

}

void handleEndSimulation(FILE* logFile, struct Queue* priority) {
    if(FIN_TIME == 10000000) {
        while (priority->front != NULL)
        {
            priority.remove(); 
        }
        
        }
    }
}








//                                 IMPLEMENTATION OF DISCRETE EVENT SIMULATOR


int main(int argc, char **argv) {
    struct Queue* CPU = createQueue();
    struct Queue* disk1 = createQueue();
    struct Queue* disk2 = createQueue();
    struct Queue* network = createQueue();
    struct PQueue* priority = createQueue(); 
    FILE* logFile = createLogFile("log-file");

    
}
