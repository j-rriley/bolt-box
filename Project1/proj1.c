#include <stdlib.h>
#include <stdio.h> 
#include <time.h>
#include <string.h>


//                               DATA BASED OFF CONFIG FILE
#define SEED 2
#define INIT_TIME 0 
#define FIN_TIME 50000
#define ARRIVE_MIN 60
#define ARRIVE_MAX 65
#define QUIT_PROB .1
#define NETWORK_PROB .1
#define CPU_MIN 50
#define CPU_MAX 55
#define DISK1_MIN 31
#define DISK1_MAX 41
#define DISK2_MIN 10
#define DISK2_MAX 43
#define NETWORK_MIN 30
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
    int totalJobs;
    int size; 
    int occupied; 
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
    q->size = 0;
    q->totalJobs = 0; 
    return q; 
} 
  
// The function to add an event to a FIFOQueue 
void add(struct FIFOQueue* q, struct Event* event) 
{ 
    
    // If FIFOQueue is empty, then new node is front and rear both 
    if (q->rear == NULL) { 
        q->front = event; 
        q->rear = event; 
        q->size =1; 
        q->totalJobs = (q->totalJobs)+1;
        return; 
    } 

    if(q->front == NULL) {
        q->front = event;
        q->rear = event; 
        q->size = 1; 
    }
  
    // If it is not empty, add the new node at the end of FIFOQueue and change rear 
    q->size = q->size +1; 
    q->rear->nextFIFO = event; 
    q->rear = event; 
    q->totalJobs = (q->totalJobs)+1;
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
    q->size = q->size -1;
    // If front becomes NULL, then change rear also as NULL 
    if (q->front == NULL) {
        q->rear = NULL; 
        q->size = 0; 
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
            //if we are at the rear, add the event as the new rear
            if(q->rear == placement)  {
                placement->nextPQ = event;
                q->rear =event;
                sorted = 0;
                return;
            }

            //if the next event's time is greater than or equal to mine
            if(placement->nextPQ->time >= timeOfNew) {
                struct Event* temp = placement->nextPQ; 
                placement->nextPQ = event;
                event->nextPQ = temp;  
                sorted = 0; 
                return; 
            }
            //we cannot sort here
            sorted = 1; 
            placement = placement->nextPQ; 
            continue; 
        }

        //if the place check is the same as the event trying to be added... we can sort
        if(placement->time == timeOfNew) {

            //if we are not at the rear, that means there is an event after that we must point to 
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
        //pretty sure this is unnecessary but...
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
        fprintf(logFile, ("At time %d, process %d enters system.\n"), eTime, eNum);
        return;
    }
    if(event->eventType == 1) {
        fprintf(logFile, ("At time %d, process %d enters CPU.\n"), eTime, eNum);
        return;
    }
    if(event->eventType == 2) {
        fprintf(logFile, ("At time %d, process %d exit CPU.\n"), eTime, eNum);
        return;
    }
    if(event->eventType == 3) {
        fprintf(logFile, ("At time %d, process %d begins I/O on Disk 1.\n"), eTime, eNum);
        return;
    }
    if(event->eventType == 4) {
        fprintf(logFile, ("At time %d, process %d ends I/O on Disk 1.\n"), eTime, eNum);
        return;
    }
    if(event->eventType == 5) {
        fprintf(logFile, ("At time %d, process %d begins I/O on Disk 2.\n"), eTime, eNum);
        return;
    }
    if(event->eventType == 6) {
        fprintf(logFile, ("At time %d, process %d ends I/O on Disk 2.\n"), eTime, eNum);
        return;
    }
    if(event->eventType == 7) {
        fprintf(logFile, ("At time %d, process %d begins I/O on network.\n"), eTime, eNum);
        return;
    }
    if(event->eventType == 8) {
        fprintf(logFile, ("At time %d, process %d ends I/O on network.\n"), eTime, eNum);
        return;
    }
    if(event->eventType == 9) {
        fprintf(logFile, ("At time %d, process %d exits the system.\n"), eTime, eNum);
        return;
    }
    if(event->eventType == 10) {
        fprintf(logFile, ("At time %d, process %d ends the simulation.\n"), eTime, eNum);
        return;
    }
    else {
        fprintf(logFile, ("Something went wrong, event type %d\n"), event->eventType);
        return;
    }
}


///////////////////////// stats stuff

struct STATS
{
    struct FIFOQueue* component; 
    double averageSize; 
    double maxSize;  
        struct Event* forEventNumSIZE; 
        struct FIFOQueue* sizes;  //holds size of queue each time event is popped 

    double avgResponseTime;
    double maxResponseTime;
        int entered;
        int exit; 
        struct Event* forEventNumRTIME; 
        struct FIFOQueue* responseTimes;  //add to response time each

    double utilization;
        int totalResponseTime; 

    double throughput;
        int jobs; 
};


struct STATS* createSTATS(struct FIFOQueue* component) {
    struct STATS* new = (struct STATS*)malloc(sizeof(struct STATS));
    new->sizes = createQueue();
    new->responseTimes = createQueue();
    new->component = component;
    new->jobs = 0; 
    return new; 
}

struct STATS* updateStats(struct STATS* component1, struct Event* event) {
    component1->jobs = component1->jobs +1;
    int toUpdate = event->eventType;
    if((toUpdate == 1) || (toUpdate == 3) || (toUpdate == 5)|| (toUpdate == 7)) {
        component1->entered = event->time;
    }
    if(toUpdate == 2 || toUpdate == 4 || toUpdate == 6 || toUpdate == 8) {
        component1->exit = event->time; 
        int difference = (component1->exit)-(component1->entered);
        component1->forEventNumRTIME = newEvent(difference);
        add(component1->responseTimes, component1->forEventNumRTIME);
    }

    //find size of input queue at this event and add to queue
    struct Event* sizeIsAdded = newEvent(component1->component->size);
    component1->forEventNumSIZE = sizeIsAdded; 
    add(component1->sizes, component1->forEventNumSIZE);
    return component1; 
}

void finishStats(FILE* statsFile, struct STATS* completed, int type) {
    //get average and maximum size of queue
    int currentSize; 
    int biggestSize = -1;
    int totalSize = 0;
    int jobs = 0; 
    //event to check
    struct Event* check; 
    int doWeCalculate = 0; 
    if(completed->sizes->front == NULL) {
        doWeCalculate = 1; 
    }

    while(completed->sizes->front != NULL) {
        check = removeQ(completed->sizes);
        currentSize = check->eventNum;
        totalSize = totalSize + currentSize;
        if(currentSize > biggestSize) {
            biggestSize = currentSize;
        }
        jobs = jobs +1;
    }
    
    completed->jobs = jobs;
    completed->throughput = (completed->jobs)/FIN_TIME;
    if(doWeCalculate == 0) {
    completed->averageSize = (totalSize/jobs); 
    completed->maxSize = biggestSize; 
    }
    else {
        completed->averageSize = 0; 
        completed->maxSize = 0; 
    }

    int currentResponseTime;
    int biggestResponseTime = -1;
    int totalResponseTime = 0; 
    doWeCalculate = 0; 
    if(completed->responseTimes->front == NULL)
    {
        doWeCalculate = 1; 
    }

    //get average and maximum response time of queue and find utilization
    while(completed->responseTimes->front != NULL) {
        check = removeQ(completed->responseTimes);
        currentResponseTime = check->eventNum;
        totalResponseTime = totalResponseTime + currentResponseTime;
        if(currentResponseTime > biggestResponseTime) {
            biggestResponseTime = currentResponseTime;
        }
    }
    if(doWeCalculate == 0) {
        completed->avgResponseTime = (totalResponseTime/jobs);
        completed->maxResponseTime = biggestResponseTime; 
    }
    else {
        completed->avgResponseTime = 0; 
        completed->maxResponseTime = 0; 
    }

    completed->utilization = (totalResponseTime/FIN_TIME);
    if(jobs != 0){
    completed->throughput = (jobs/FIN_TIME);
    }
    else {
        completed->throughput = 0; 
    }

    /*char* queueType;
    if(type == 1) {
        char queueType1[3] = {'C','P','U'}; 
        queueType = queueType1;
    }
    if(type == 2) {
        char queueType2[5] = {'D','i','s', 'k', '1'}; 
        queueType = queueType2;
    }
    if(type == 3) {
        char queueType3[5] = {'D','i','s', 'k', '2'}; 
        queueType = queueType3; 
    }
    else {
        char queueType4[7] = {'N','e','t', 'w', 'o', 'r', 'k'}; 
        queueType = queueType4; 
    }
    */
    fprintf(statsFile, ("Stats for %d Queue: \n"), type);
    fprintf(statsFile, ("Average size of %d- %f\n"), type, completed->averageSize);
    fprintf(statsFile, ("Max size of %d- %f\n"), type, completed->maxSize);
    fprintf(statsFile, ("Utilization of %d- %f\n"), type, completed->utilization);
    fprintf(statsFile, ("Average response time of %d- %f\n"), type, completed->avgResponseTime);
    fprintf(statsFile, ("Max response time of %d- %f\n"), type, completed->maxResponseTime);
    fprintf(statsFile, ("Throughput of %d- %f\n"), type, completed->throughput);
}

//----------------------------------------> RANDOM UTILITY TASKS

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

//checkWhichDisk to choose for cpuexit
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
        int disk1Count = 0;
        int disk2Count = 0; 
        struct Event* placement1 = disk1->front; 
        struct Event* placement2 = disk2->front;; 

        while(placement1 != NULL) {
            disk1Count = disk1Count +1; 
            placement1 = placement1->nextFIFO; 
        } 

        while(placement2 != NULL) {
            disk2Count = disk2Count +1; 
            placement2 = placement2->nextFIFO; 
        } 

        if(disk1Count <= disk2Count) {
            whichDisk = 1; 
            return whichDisk; 
        }
        else{
            whichDisk = 2; 
        }
    }
    return whichDisk; 
}




// ----------------------------------->NEW PROGRAM STRUCTURE
//event handlers with designated keys: 
//event type 0

void handleArrival(struct PQueue* priority, struct FIFOQueue* CPU, struct Event* job) {

    //if the CPU is occupied or the Queue is not empty
    if((CPU->occupied == 0) || (CPU->front != NULL)) {
        add(CPU, job);
    }

    //if the CPU is not occupied and the queue is empty, we can immediately send the event to the CPU
    else{
        //we change the event to process_arrive_CPU and add it to the priority queue 
        job->eventType = 1; 
        CPU->occupied = 0; 
        addPQ(priority, job);
    }

    //create new event for arrival to continue with processes
    struct Event* new = newEvent(job->eventNum+1);
    new->eventType = 0; 
    new->time = job->time + determineTime(ARRIVE_MIN, ARRIVE_MAX);
    addPQ(priority, new);
}

//event type 1
void handleArrivalCPU(struct PQueue* priority, struct Event* job) {
    job->eventType = 2; 
    job->time = job->time + determineTime(CPU_MIN, CPU_MAX);
    addPQ(priority, job);
}


//event type 2
void handleFinishCPU(struct PQueue* priority, struct FIFOQueue* CPU, struct FIFOQueue* disk1, struct FIFOQueue* disk2, struct FIFOQueue* network, struct Event* job) {
     CPU->occupied = 1; 
     //will it exit the event handler? 
    int quitProb = determineProbability(QUIT_PROB);
    //if yes, nothing else needs to be done here. add the next to the priority queue for processing because the CPU is free. 
    if(quitProb == 0) {
       job->eventType = 9;  
       addPQ(priority, job);
    }

    //if no, check if it will go to network or disk
    else {


        int networkProb = determineProbability(NETWORK_PROB);

        if(networkProb == 0) {
                
         //if the network is freed up, immediately add new event to Pqueue
                     if((network->occupied == 1) || (network->front == NULL)) { 
                        job->eventType = 7;
                        addPQ(priority, job);
                        network->occupied = 0; 
                    }

                //if the network has events waiting, add it to the network queue for processing
                    else{
                        add(network, job); 
                    }  

            }
           
             //if not network, it will go to a disk. which disk? call disk function
            else{

                int diskChoice = checkWhichDisk(disk1, disk2);


                if(diskChoice == 1) {
                    //if disk1 is freed up, immediately add job to Pqueue
                    if((disk1->occupied == 1) || (disk1->front == NULL)) {
                        job->eventType = 3;
                        addPQ(priority, job);
                        disk1->occupied = 0; 
                    }

                    //if the disk1 has events waiting, add it to the disk1 queue for processing
                    else{
                        add(disk1, job);
                    }
                }


                //diskChoice is 2
                else{

                    //if disk2 is freed up, immediately add job to Pqueue (consider it processed)
                     if((disk2->occupied == 1) || (disk2->front == NULL)) { 
                        job->eventType = 5;
                        addPQ(priority, job);
                        disk2->occupied = 0;
                    }

                    //if the disk2 has events waiting, add it to the disk2 queue for processing
                    else{
                        add(disk2, job); 
                    }

                }
            }
        }       

    if(CPU->front != NULL) {
        struct Event* nextProcessed = removeQ(CPU); 
        if(nextProcessed->eventType != 1) {
            nextProcessed->eventType = 1; 
            nextProcessed->time = job->time; 
        }
        addPQ(priority, nextProcessed);
        CPU->occupied = 0;
    }

}

//event type 3
void handleArrivalDisk1(struct PQueue* priority, struct Event* job) {
    job->time = job->time + determineTime(DISK1_MIN, DISK1_MAX);
    job->eventType = 4; 
    addPQ(priority, job);
}

//event type 4
void handleFinishDisk1(struct PQueue* priority, struct FIFOQueue* CPU, struct FIFOQueue* disk1, struct Event* job) {
    disk1->occupied = 1; 
    //if CPU occupied or queue is non empty
    int count = 0; 
    while(count != 1) {
    if((CPU->occupied == 0) || (CPU->front != NULL)) {
        add(CPU, job);
    }
    else {
        job->eventType = 1;
        addPQ(priority, job);
        CPU->occupied = 0; 
        }
        count= count+1; 
    }

    if(disk1->front != NULL) {
        struct Event* nextProcessed = removeQ(disk1);
        addPQ(priority, nextProcessed);
        disk1->occupied = 0; 
    }
}

//event type 5
void handleArrivalDisk2(struct PQueue* priority, struct Event* job) {
    job->time = job->time + determineTime(DISK2_MIN, DISK2_MAX);
    job->eventType = 6; 
    addPQ(priority, job);

}

//event type 6
void handleFinishDisk2(struct PQueue* priority, struct FIFOQueue* CPU, struct FIFOQueue* disk2, struct Event* job) {
    disk2->occupied = 1; 
    //if CPU occupied or queue is non empty
    int count = 0;
    while(count != 1) {
    if((CPU->occupied == 0) || (CPU->front != NULL)) {
        add(CPU, job);
    }
    else {
        job->eventType = 1;
        addPQ(priority, job);
        CPU->occupied = 0; 
        } 
        count = count +1; 
    }
    if(disk2->front != NULL) {
        struct Event* nextProcessed = removeQ(disk2);
        addPQ(priority, nextProcessed);
        disk2->occupied = 0; 
    }
}

//event type 7
void handleArrivalNetwork(struct PQueue* priority, struct Event* job) {
    job->time = job->time + determineTime(NETWORK_MIN, NETWORK_MAX);
    job->eventType = 8; 
    addPQ(priority, job);

}

//event type 8
void handleFinishNetwork(struct PQueue* priority, struct FIFOQueue* CPU, struct FIFOQueue* network, struct Event* job) {
    network->occupied = 1; 
    //if CPU occupied or queue is non empty
    int count = 0; 
    while(count != 1) {
    if((CPU->occupied == 0) || (CPU->front != NULL)) {
        add(CPU, job);
    }
    else {
        job->eventType = 1;
        addPQ(priority, job);
        CPU->occupied = 0; 
    }
    count = count+1;
    }

    if(network->front != NULL) {
        struct Event* nextProcessed = removeQ(network);
        addPQ(priority, nextProcessed);
        network->occupied = 0; 
    }
}

//event type 9
void handleExitSystem(struct Event* job) {
    return; 
}

//event type 10
void handleFinish(FILE* statsFile, FILE* logFile, struct PQueue* priority, struct Event* job) {
    priority->front = NULL; 
    fclose(logFile);
    fclose(statsFile);
}

/*void generateStatistics(struct Queue* q) {
    
}
*/

//                                 IMPLEMENTATION OF DISCRETE EVENT SIMULATOR


int main(int argc, char **argv) {
    //readConfigFile("config.txt");
    time_t seed; 
    srand((unsigned) time(&seed));
    struct FIFOQueue* CPU = createQueue();
    CPU->occupied = 1; 
    struct FIFOQueue* disk1 = createQueue();
    disk1->occupied = 1; 
    struct FIFOQueue* disk2 = createQueue();
    disk2->occupied = 1; 
    struct FIFOQueue* network = createQueue();
    network->occupied = 1; 
    struct PQueue* priority = createPQueue(); 
    FILE* logFile = createLogFile("log-file");
    FILE* statsFile = createLogFile("stats-file");
    struct STATS* sCPU = createSTATS(CPU);
    struct STATS* sDisk1 = createSTATS(disk1);
    struct STATS* sDisk2 = createSTATS(disk2);
    struct STATS* sNetwork = createSTATS(network);
    
    //initializing priority queue
    struct Event* event1 = newEvent(1);
    event1->eventType = 0; 
    event1->time = INIT_TIME; 
    addPQ(priority, event1);
    
    //endpoint for priority queue
    struct Event* eventLast = newEvent(0);
    eventLast->eventType = 10;
    eventLast->time = FIN_TIME; 
    addPQ(priority, eventLast);

    //event to be pulled
    struct Event* event;

    while(priority->front != NULL) {

        event = removePQ(priority);

        if(event->eventType == 0) {
            addLog(logFile, event);
            handleArrival(priority, CPU, event);
            continue;
        }
        
        if(event->eventType == 1) {
            addLog(logFile, event);
            sCPU = updateStats(sCPU, event);
            handleArrivalCPU(priority, event);
            continue;
        }

        if(event->eventType == 2) {
            addLog(logFile, event);
            sCPU = updateStats(sCPU, event);
            handleFinishCPU(priority, CPU, disk1, disk2, network, event);
            continue;
        }

        if(event->eventType == 3) {
            addLog(logFile, event);
            sDisk1 = updateStats(sDisk1, event);
            handleArrivalDisk1(priority, event);
            continue;
        }

        if(event->eventType == 4) {
            addLog(logFile, event);
            sDisk1 = updateStats(sDisk1, event);
            handleFinishDisk1(priority, CPU, disk1, event);
            continue;
            
        }
        if(event->eventType == 5) {
            addLog(logFile, event);
            sDisk2 = updateStats(sDisk2, event);
            handleArrivalDisk2(priority, event);
            continue;
            
        }
        if(event->eventType == 6) {
            addLog(logFile, event);
            sDisk2= updateStats(sDisk2, event);
            handleFinishDisk2(priority, CPU, disk2, event);
            continue;
            
        }
        if(event->eventType == 7) {
            addLog(logFile, event);
            sNetwork = updateStats(sNetwork, event);
            handleArrivalNetwork(priority, event);
            continue;
            
        }
        if(event->eventType == 8) {
            addLog(logFile, event);
            sNetwork = updateStats(sNetwork, event);
            handleFinishNetwork(priority, CPU, network, event);
            continue;
            
        }
        if(event->eventType == 9) {
            addLog(logFile, event);
            handleExitSystem(event);
            continue;
            
        }
        else {
            addLog(logFile, event);
            finishStats(statsFile, sCPU, 1);
            finishStats(statsFile, sDisk1, 2);
            finishStats(statsFile, sDisk2, 3);
            finishStats(statsFile, sNetwork, 4);
            handleFinish(statsFile, logFile, priority, event); 
            
        }
    }
    return 0;
}
