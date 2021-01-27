    /*  Program 2: Using a single process but total of 9 threads (including 
        parent thread) performing same activities carried out by individual 
        processes in program 1. Overall, same work but two differnt solutions. */


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
//#define TEST
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

struct shared_val shared_val; 

pthread_t thread_pool[8]; 



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
    if((end.tv_nsec - shared_val.start.tv_sec) < 0) {
        elapsed = (end.tv_sec - shared_val.start.tv_sec - 1) + (BILLION+ end.tv_nsec - shared_val.start.tv_nsec) / BILLION;
    }
    else {
        elapsed = (end.tv_sec - shared_val.start.tv_sec) + (end.tv_nsec - shared_val.start.tv_nsec) / BILLION;
    }
    return elapsed; 
}

                                        /*    Function to initialize the shared  *
                                         * memory reporter, can be called every 10 cycles */

void set_up_report() {
    strcpy(shared_val.log[0], "System Time: ");
    strcpy(shared_val.log[1], "Signal 1 Sent: ");
    strcpy(shared_val.log[2], "Signal 2 Sent: ");
    strcpy(shared_val.log[3], "Signal 1 Received: "); 
    strcpy(shared_val.log[4], "Signal 2 Received: "); 
    strcpy(shared_val.log[5], "Avg Time Between Signal 1: ");
    strcpy(shared_val.log[6], "Avg Time Between Signal 2: ");
}






                                        /*    Three signal generating processes   *
                                         * that are spawned by the parent process */

//Responsibilities: Wait a random interval between .01 and .1 seconds, randomly choose
//                  between SIGUR1 or SIGUR2, send chosen signal to all child processes
//                  in the program, increment a shared signal sent counter, one for each
//                  signal type

void* signal_generating(void* arg) { 
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

            
            pthread_kill(thread_pool[7], signal);


        
            if(signal == SIGUSR1) {
                pthread_kill(thread_pool[0], signal);
                pthread_kill(thread_pool[1], signal);
                pthread_mutex_lock(&shared_val.mutex); //acquire lock
                shared_val.signal1_sent++;  //sigusr1 sent counter ++
                #ifdef TEST
                printf("Incrementing sent counter for signal 1 by 1!\n");
                printf("New value: %d\n", shared_val.signal1_sent);
                printf("Current time: %.10f\n", get_time_elapsed());
                #endif 
                pthread_mutex_unlock(&shared_val.mutex); //release lock
            }

            if(signal == SIGUSR2) {
                pthread_kill(thread_pool[2], signal);
                pthread_kill(thread_pool[3], signal);
                pthread_mutex_lock(&shared_val.mutex); //acquire lock 
                shared_val.signal2_sent++; //sigusr2 sent counter ++
                #ifdef TEST
                printf("Incrementing sent counter for signal 2 by 1!\n");
                printf("New value: %d\n", shared_val.signal2_sent);
                printf("Current time: %.10f\n", get_time_elapsed());
                #endif 
                pthread_mutex_unlock(&shared_val.mutex); //release lock 
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



void* loop_for_signal1(void* arg) {
    sigset_t sigset; 
    int signal;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);  
    pthread_sigmask(SIG_BLOCK, &sigset, NULL);
    while(1) {
        sigwait(&sigset, &signal);
        if(signal == SIGUSR1) {
            pthread_mutex_lock(&shared_val.mutex); //acquire lock
            shared_val.signal1_received++; //increment sigusr1_received
            #ifdef TEST
            printf("Incrementing received counter for signal 1 by 1!\n");
            printf("New value: %d\n", shared_val.signal1_received);
            printf("Current time: %.10f\n", get_time_elapsed());
            #endif
            pthread_mutex_unlock(&shared_val.mutex); //release lock 
        }
    }
}

void* loop_for_signal2(void* arg) {
    sigset_t sigset; 
    int signal;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR2);  
    pthread_sigmask(SIG_BLOCK, &sigset, NULL);
    while(1) {
        sigwait(&sigset, &signal);
        if(signal == SIGUSR2) {
            pthread_mutex_lock(&shared_val.mutex); //acquire lock
            shared_val.signal2_received++; //increment sigusr1_received
            #ifdef TEST
            printf("Incrementing received counter for signal 2 by 1!\n");
            printf("New value: %d\n", shared_val.signal2_received);
            printf("Current time: %.10f\n", get_time_elapsed());
            #endif
            pthread_mutex_unlock(&shared_val.mutex); //release lock 
        }
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


void* reporting_process(void* arg) {
    sigset_t sigset; 
    int signal; 
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    sigaddset(&sigset, SIGUSR2);
    pthread_sigmask(SIG_BLOCK, &sigset, NULL);
    int last = 0;  
    int current; 
    int cycle_counts = 0; 
    double current_average1 = 0; 
    double current_average2 = 0; 
    while(1) {      
        sigwait(&sigset, &signal); 
        if(signal == SIGUSR1) {
            pthread_mutex_lock(&shared_val.mutex); //acquire lock
            shared_val.reporting_signals1++; 
            pthread_mutex_unlock(&shared_val.mutex); //acquire lock
        }
        if(signal == SIGUSR2) {
            pthread_mutex_lock(&shared_val.mutex); //acquire lock
            shared_val.reporting_signals2++; 
            pthread_mutex_unlock(&shared_val.mutex); //acquire lock
        }
        current = shared_val.reporting_signals1 + shared_val.reporting_signals2; 
        if( (current - last) >= 10 ) {
            cycle_counts++; 
            current_average1 = current_average1 + (((get_time_elapsed()/(shared_val.reporting_signals1)) /cycle_counts)); 
            current_average2 = current_average2 + (((get_time_elapsed()/(shared_val.reporting_signals2)) /cycle_counts)); 
            printf("\n CYCLE NUMBER %d \n", cycle_counts); 
            printf("%s %lf\n", shared_val.log[0], get_time_elapsed()); 
            printf("%s %d\n", shared_val.log[1], shared_val.signal1_sent);
            printf("%s %d \n", shared_val.log[2], shared_val.signal2_sent);
            printf("%s %d \n", shared_val.log[3], shared_val.signal1_received);
            printf("%s %d \n", shared_val.log[4], shared_val.signal2_received);
            printf("%s %f \n", shared_val.log[5], current_average1);
            printf("%s %f \n", shared_val.log[6], current_average2);
            last = current; 
        }
    }
}

                                        /*    One parent process that will  *
                                         *     spawn all other processes    */

//Responsibilities: INITIALIZATION BY -> spawning child processes, setting up shared 
//                  variables (counters) between processes, sets up mutexes, controls
//                  the execution duration of the program. 

void* parent_process(void* arg) {
    //Initializing memory and lock
    if(pthread_mutex_init(&shared_val.mutex, NULL) != 0) {
        printf("Initializing mutex lock for variables failed.");
        exit(1);
    }

    shared_val.signal1_sent = 0; 
    shared_val.signal2_sent = 0;
    shared_val.signal1_received = 0; 
    shared_val.signal2_received = 0; 
    shared_val.reporting_signals1 = 0;
    shared_val.reporting_signals2 = 0;

    #ifdef TEST
    printf("Shared memory has been initialized.\n");
    #endif

    //Setting up the contents of the report 
    set_up_report(); 
    #ifdef TEST
    printf("Report has been set up.\n");
    #endif
    
    //Start clock when threads are about to be created
    clock_gettime(CLOCK_REALTIME, &shared_val.start); 
    #ifdef TEST
    printf("Clock is starting!\n");
    #endif

    //Creating threads 
    int i; 
    for(i = 0; i < 8; i++) { 
        if( i == 0 || i == 1 ) { 
            if(pthread_create(&thread_pool[i], NULL, loop_for_signal1, NULL) != 0) {
                printf("Thread creating for signal 1 loop failed.\n");
                exit(1);
            }       
            #ifdef TEST
            printf("Signal handling 1 thread created.\n");
            #endif 
        }
        if(i == 2 || i == 3) {
            if(pthread_create(&thread_pool[i], NULL, loop_for_signal2, NULL) != 0) {
                printf("Thread creating for signal 1 loop failed.\n");
                exit(1);
            }
            #ifdef TEST
            printf("Signal handling 2 thread created.\n");
            #endif      
        }
        if( i == 4 || i == 5 || i == 6 ) {
            if(pthread_create(&thread_pool[i], NULL, signal_generating, NULL) != 0) {
                printf("Thread creating for signal generator %d failed.\n", i-3);
                exit(1);
            }
            #ifdef TEST
            printf("Signal generating process %d created.\n", i-3);
            #endif
        }
        if(i == 7) {
            if(pthread_create(&thread_pool[i], NULL, reporting_process, NULL) != 0) {
                printf("Thread creating for reporting thread failed.\n");
                exit(1);
            }
            #ifdef TEST
            printf("Reporting process created.\n");
            #endif
        } 
    }
    #ifdef TIME_EXECUTION
    if(get_time_elapsed() <= 30) {
        double time_needed = 30 - get_time_elapsed(); 
        sleep(time_needed); 
    }

    for(i = 0; i < 8; i++) { //end threads
        pthread_cancel(thread_pool[i]);
    }

    #ifdef TEST
    printf("Ending the program now that our conditions have been met.\n");
    printf("Time = %f\n", get_time_elapsed());
    #endif
    #endif

    #ifdef SIGNAL_EXECUTION
    while(!((shared_val.signal1_sent + shared_val.signal2_sent + shared_val.signal1_received + shared_val.signal2_received + shared_val.reporting_signals1 + shared_val.reporting_signals2) >= 100000)) {
        sleep(10); 
        continue; 
    }

    #ifdef TEST
    printf("Time to cleanup! \n");
    #endif
    for(i = 0; i < 8; i++) { //end threads
        pthread_cancel(thread_pool[i]);
    }
    #ifdef TEST
    printf("Ending the program now that our conditions have been met.\n");
    printf("Signals in total = %d\n", (shared_val.signal1_sent + shared_val.signal2_sent));
    #endif
    #endif
    
    
    #ifdef TEST
    printf("Goodbye!");
    #endif
    exit(0);
}



int main() {
    srand(time(NULL));
    pthread_t parent; 
    if(pthread_create(&parent, NULL, parent_process, NULL) != 0) {
        printf("Thread creating for parent thread failed.\n");
        exit(1);
    }
    #ifdef TEST
    printf("Parent thread created.\n");
    #endif

    while(1) {}
}