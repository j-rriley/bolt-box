    /* Program 1: Using a parent process, four signal-handling 
    processes, three signal-generating processes, and a reporting process.
    Total of nine different processes. */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include<sys/wait.h> 
#include <pthread.h> 
#include <string.h>
#include <math.h>
#define TEST
#define TIME_EXECUTION
//#define SIGNAL_EXECUTION
#define BILLION 1000000000

struct shared_val {
    pthread_mutex_t mutex; 
    int signal1_sent; 
    int signal2_sent;
    int signal1_received; 
    int signal2_received; 
    char log[7][40];
    struct timespec start; 
    int reporting_signals1;
    int reporting_signals2; 

}; 

struct shared_val *shared_pointer; 
pthread_mutexattr_t attr; 
int shm_id; 

pid_t pids[8]; 



                                        /*    Function to generate signal 1  *
                                         *           or signal 2             */

int random_signal() {
   return (rand() % 2) + 1; 
}

                                        /*    Function to sleep for a random  *
                                         *      interval between .1 and .01   */
void sleep_random_interval() {
    double random = rand(); 
    double interval = (double) fmod(random, 0.09) + .01; 
    int microseconds = interval * 1000000;
    #ifdef TEST
    printf("Sleeping for %f seconds.\n", interval);
    #endif
    usleep(microseconds);
}

                                        /*    Function to get time elapsed from the start */
double get_time_elapsed() {
    struct timespec end; 
    double elapsed; 
    clock_gettime(CLOCK_REALTIME, &end); 
    if((end.tv_nsec - shared_pointer->start.tv_sec) < 0) {
        elapsed = (end.tv_sec - shared_pointer->start.tv_sec - 1) + (BILLION+ end.tv_nsec - shared_pointer->start.tv_nsec) / BILLION;
    }
    else {
        elapsed = (end.tv_sec - shared_pointer->start.tv_sec) + (end.tv_nsec - shared_pointer->start.tv_nsec) / BILLION;
    }
    return elapsed; 
}

                                        /*    Function to initialize the shared  *
                                         * memory reporter, can be called every 10 cycles */

void set_up_report() {
    strcpy(shared_pointer->log[0], "System Time: ");
    strcpy(shared_pointer->log[1], "Signal 1 Sent: ");
    strcpy(shared_pointer->log[2], "Signal 2 Sent: ");
    strcpy(shared_pointer->log[3], "Signal 1 Received: "); 
    strcpy(shared_pointer->log[4], "Signal 2 Received: "); 
    strcpy(shared_pointer->log[5], "Avg Time Between Signal 1: ");
    strcpy(shared_pointer->log[6], "Avg Time Between Signal 2: ");
}


void unblock_sigusrs() {
    sigset_t sigset; 
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    sigaddset(&sigset, SIGUSR2); 
    sigprocmask(SIG_UNBLOCK, &sigset, NULL);
}

void block_sigusrs() {
    sigset_t sigset; 
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    sigaddset(&sigset, SIGUSR2); 
    sigprocmask(SIG_BLOCK, &sigset, NULL);
}

void block_sigusr1() {
    sigset_t sigset; 
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sigset, NULL);
}

void block_sigusr2() {
    sigset_t sigset; 
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR2);
    sigprocmask(SIG_BLOCK, &sigset, NULL);
}



                                        /*    Three signal generating processes   *
                                         * that are spawned by the parent process */

//Responsibilities: Wait a random interval between .01 and .1 seconds, randomly choose
//                  between SIGUR1 or SIGUR2, send chosen signal to all child processes
//                  in the program, increment a shared signal sent counter, one for each
//                  signal type

void signal_generating() { 
        #ifdef TEST
        printf("Time to generate signals, but first sleeping.\n");
        #endif
        int choice, signal; 
        while(1) {
            sleep_random_interval(); //sleep for random interval between .01 and .1
            choice = random_signal(); //signal = random signal
            if(choice == 1) {
                signal = SIGUSR1; 
            }
            if(choice == 2) {
                signal = SIGUSR2; 
            }
            #ifdef TEST
            printf("We picked a signal. Signal choice: %d... Preparing to send.\n", signal);
            #endif
            kill(0, signal); //kill (0, signal)
        
            if(signal == SIGUSR1) {
                pthread_mutex_lock(&shared_pointer->mutex); //acquire lock
                shared_pointer->signal1_sent++;  //sigusr1 sent counter ++
                #ifdef TEST
                printf("Incrementing sent counter for signal 1 by 1!\n");
                printf("New value: %d\n", shared_pointer->signal1_sent);
                printf("Current time: %.10f\n", get_time_elapsed());
                #endif 
                pthread_mutex_unlock(&shared_pointer->mutex); //release lock
            }

            if(signal == SIGUSR2) {
                pthread_mutex_lock(&shared_pointer->mutex); //acquire lock 
                shared_pointer->signal2_sent++; //sigusr2 sent counter ++
                #ifdef TEST
                printf("Incrementing sent counter for signal 2 by 1!\n");
                printf("New value: %d\n", shared_pointer->signal2_sent);
                printf("Current time: %.10f\n", get_time_elapsed());
                #endif 
                pthread_mutex_unlock(&shared_pointer->mutex); //release lock 
            }
        }
}

                                        /*    Four signal handling processes   *
                                         * that are spawned by the parent process */

//Responsibilities: Two will handle SIGUSR1, two will handle SIGUSR2. First, hooks up
//                  a signal handling process for its associated signal, and block the
//                  other signal type by modifying its signal mask. When a signal arrives, 
//                  the signal handler increments a shared signal received counter associated
//                  with signal type. Sit in a loop, preferably with sleep() to prevent wasted
//                  cpu cycles. 



void signal_handler(int signal) {
    if(signal == SIGUSR1) {
        pthread_mutex_lock(&shared_pointer->mutex); //acquire lock
        shared_pointer->signal1_received++; //increment sigusr1_received
        #ifdef TEST
        printf("Incrementing received counter for signal 1 by 1!\n");
        printf("New value: %d\n", shared_pointer->signal1_received);
        printf("Current time: %.10f\n", get_time_elapsed());
        #endif
        pthread_mutex_unlock(&shared_pointer->mutex); //release lock 
    }
}


void signal_handler2(int signal) {
    if(signal== SIGUSR2) {
        pthread_mutex_lock(&shared_pointer->mutex); //acquire lock
        shared_pointer->signal2_received++; //increment sigusr2_received
        #ifdef TEST
        printf("Incrementing received counter for signal 2 by 1!\n");
        printf("New value: %d\n", shared_pointer->signal2_received);
        printf("Current time: %.10f\n", get_time_elapsed());
        #endif
        pthread_mutex_unlock(&shared_pointer->mutex); //release lock 
    }
}

void loop_for_signal1() {
    unblock_sigusrs();
    block_sigusr2(); 
    while(1) {
        signal(SIGUSR1, signal_handler); 
        pause(); 
    }
}

void loop_for_signal2() {
    unblock_sigusrs();
    block_sigusr1(); 
    while(1) {
        signal(SIGUSR2, signal_handler2); 
        pause(); 
    }
}


                                        /*  One reporting process that is *
                                         *  spawned by the parent process */

//Responsibilities: Handles both type of signals, maintains private counter of how many
//                  signals are received, keeps track of the time of occurrence of each signal
//                  type, with memory lasting 10 signals. Everytime 10 signals are handled, it
//                  reports the system time, the shared counters of how many signals of each 
//                  type have been sent/received, and reports the avg time between receptions of
//                  each signal type for the given 10 signal interval 

void reporting_handler1(int sig) {
    if(sig == SIGUSR1) {
        pthread_mutex_lock(&shared_pointer->mutex); //acquire lock
        shared_pointer->reporting_signals1++; 
        pthread_mutex_unlock(&shared_pointer->mutex); //acquire lock
    }
}

void reporting_handler2(int sig) {
    if(sig == SIGUSR2) {
        pthread_mutex_lock(&shared_pointer->mutex); //acquire lock
        shared_pointer->reporting_signals2++; 
        pthread_mutex_unlock(&shared_pointer->mutex); //acquire lock
    }
}

void reporting_process() {
    unblock_sigusrs(); 
    int last = 0;  
    int current; 
    int cycle_counts = 0; 
    double current_average1 = 0; 
    double current_average2 = 0; 
    while(1) {      
        current = shared_pointer->reporting_signals1 + shared_pointer->reporting_signals2; 
        if( (current - last) >= 10 ) {
            cycle_counts++; 
            current_average1 = current_average1 + (((get_time_elapsed()/(shared_pointer->reporting_signals1)) /cycle_counts)); 
            current_average2 = current_average2 + (((get_time_elapsed()/(shared_pointer->reporting_signals2)) /cycle_counts)); 
            printf("\n CYCLE NUMBER %d \n", cycle_counts); 
            printf("%s %lf\n", shared_pointer->log[0], get_time_elapsed()); 
            printf("%s %d\n", shared_pointer->log[1], shared_pointer->signal1_sent);
            printf("%s %d \n", shared_pointer->log[2], shared_pointer->signal2_sent);
            printf("%s %d \n", shared_pointer->log[3], shared_pointer->signal1_received);
            printf("%s %d \n", shared_pointer->log[4], shared_pointer->signal2_received);
            printf("%s %f \n", shared_pointer->log[5], current_average1);
            printf("%s %f \n", shared_pointer->log[6], current_average2);
            last = current; 
        }
        signal(SIGUSR1, reporting_handler1);
        signal(SIGUSR2, reporting_handler2); 
        pause(); 
    }
}

                                        /*    One parent process that will  *
                                         *     spawn all other processes    */

//Responsibilities: INITIALIZATION BY -> spawning child processes, setting up shared 
//                  variables (counters) between processes, sets up mutexes, controls
//                  the execution duration of the program. 

void parent_process() {
    block_sigusrs(); 
    int exitTrue = 0;


    //Creating shared memory space 
    shm_id = shmget(IPC_PRIVATE, 
                    sizeof(struct shared_val),
                    IPC_CREAT | 0666); 
    if(shm_id < 0) {
        printf("Error creating shared memory space.\n");
        exit(1); 
    }
    #ifdef TEST
    printf("Shared memory has been created.\n");
    #endif

    //Creating shared pointer for access
    shared_pointer = (struct shared_val*) shmat(shm_id, NULL, 0);
    if(shared_pointer == (struct shared_val *) -1) {
        printf("Error creating shared pointer.\n");
        exit(1); 
    }
    #ifdef TEST
    printf("Shared pointer has been created.\n");
    #endif

    //Initializing shared memory 
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shared_pointer->mutex, &attr);
    shared_pointer->signal1_sent = 0; 
    shared_pointer->signal2_sent = 0;
    shared_pointer->signal1_received = 0; 
    shared_pointer->signal2_received = 0; 
    shared_pointer->reporting_signals1 = 0;
    shared_pointer->reporting_signals2 = 0;

    #ifdef TEST
    printf("Shared memory has been initialized.\n");
    #endif

    //Setting up the contents of the report 
    set_up_report(); 
    #ifdef TEST
    printf("Report has been set up.\n");
    #endif
    
    //Start clock when processes are about to be created
    clock_gettime(CLOCK_REALTIME, &shared_pointer->start); 
    #ifdef TEST
    printf("Clock is starting!\n");
    #endif

    //Creating processes 
    int i; 
    for(i = 0; i < 8; i++) {
        pids[i] = fork(); 
        if(pids[i] == 0) { //child processes

            if( i == 0 || i == 1 ) { 
                #ifdef TEST
                printf("Signal handling 1 process created.\n");
                #endif 
                loop_for_signal1(); 
                exit(0);
            }

            if(i == 2 || i == 3) {
                #ifdef TEST
                printf("Signal handling 2 process created.\n");
                #endif  
                loop_for_signal2(); 
                exit(0);
            }

            if( i == 4 || i == 5 || i == 6 ) {
                #ifdef TEST
                printf("Signal generating process created.\n");
                #endif
                signal_generating();
                exit(0);
            }

            if(i == 7) {
                #ifdef TEST
                printf("Reporting process created.\n");
                #endif
                reporting_process(); 
                exit(0);
            } 
        }

        else { //parent process
            if(i != 7) {
                continue; 
            }
            #ifdef TIME_EXECUTION
            if(i == 7 && get_time_elapsed() < 30) {
                double time_needed = 30 - get_time_elapsed(); 
                sleep(time_needed); 
            }
            exitTrue = 1; 
            #endif
            #ifdef SIGNAL_EXECUTION
            if(i == 7 && !((shared_pointer->signal1_sent + shared_pointer->signal2_sent + shared_pointer->signal1_received + shared_pointer->signal2_received + shared_pointer->reporting_signals1 + shared_pointer->reporting_signals2) >= 1000000)) {
                int total_signals = (shared_pointer->signal1_sent + shared_pointer->signal2_sent + shared_pointer->signal1_received + shared_pointer->signal2_received + shared_pointer->reporting_signals1 + shared_pointer->reporting_signals2);
                while(total_signals < 1000000) {
                    sleep(60); 
                    total_signals = (shared_pointer->signal1_sent + shared_pointer->signal2_sent + shared_pointer->signal1_received + shared_pointer->signal2_received + shared_pointer->reporting_signals1 + shared_pointer->reporting_signals2);
                }
                exitTrue = 1;
                break; 
            }
            #endif

            #ifdef TEST
            printf("Ending the process now that we conditions have been met.\n");
            printf("Time = %f\n", get_time_elapsed());
            #endif  
        }
    }
     
    if(exitTrue == 1) {
        #ifdef TEST
        printf("Time to cleanup! \n");
        #endif
        for(i = 0; i < 8; i++) { //kill children processes
            kill(pids[i], SIGTERM); 
        }
        #ifdef TEST
        printf("Children processes have been destroyed.\n");
        #endif

        shmdt(shared_pointer); //detach from shared memory region  
        #ifdef TEST
        printf("Shared memory has been detached.\n");
        #endif

        #ifdef TEST
        printf("Goodbye!\n");
        #endif
        exit(0);
    }
}



int main() {
    srand(time(NULL));
    parent_process(); 

}