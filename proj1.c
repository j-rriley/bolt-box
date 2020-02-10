#include <stdlib.h>
#include <stdio.h> 





//                               DATA BASED OFF CONFIG FILE
#define JOB_NUM
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



//                                   QUEUE DATA STRUCTURE 


// A linked list (LL) node to store a queue entry 
struct QNode { 
    int process; 
    struct QNode* next; 
}; 
  
// The queue, front stores the front node of LL and rear stores the last node of LL 
struct Queue { 
    struct QNode *front, *rear; 
}; 
  
// A utility function to create a new linked list node. 
struct QNode* newNode(int k) 
{ 
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode)); 
    temp->process = k; 
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
    struct QNode* temp = newNode(k); 
  
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
QNode remove(struct Queue* q) 
{ 
    // If queue is empty, return NULL. 
    if (q->front == NULL) {
        return NULL; 
    }
    // Store previous front and move front one node ahead 
    struct QNode* temp = q->front; 
    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 
    if (q->front == NULL) {
        q->rear = NULL; 
    }

    return temp; 
} 







//                                       LOG FILE CREATION


void createLogFile(String name) {

}









//                                         EVENT HANDLERS 


//When the config file is read, the jobs will be loaded one by one into the queue

void handleJobArrival(struct Queue* q, int job) {
    add(q, job);
    
}

void handleUseOfCPU() {

}

//Will it exit the system? will it do disk I/O or use the network? 

void handleCPUExit() {

}


//If it will be read by a disk: add to the disk that is free for use

void handleUseOfDisks() {

}

//Add to the network queue

void handleUseOfNetwork() {

}

void handleUseOfDisks() {

}








//                                 IMPLEMENTATION OF DISCRETE EVENT SIMULATOR


int main(int argc, char **argv) {
    struct Queue* CPU = createQueue();
    struct Queue* disk1 = createQueue();
    struct Queue* disk2 = createQueue();
    struct Queue* network = createQueue();
    struct Queue* priority = createQueue(); 
    
}