/* PROJECT 3: NETWORKED SPELL CHECKER
 *  Jaime Riley, CIS3207, LabSect 4
 * This lab is an example of the producer-
 * consumer problem we have studied in class. 
 * This solution focuses on correctness and
 * efficiency through locks and condition variables
 * to provide mutual exclusion of resources. 
 * 
 */




#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>


                        /**        Variables needed for Dictionary
                        Includes the default dictionary file, the 
                        amount of words (size of) the default dictionary,
                        the max size of a word, and the global variables 
                        of the dictionary size and dictionary name  **/

#define DEFAULT_DICTIONARY "words"
#define MAX_WORD_SIZE 20
#define MAX_PHRASE_SIZE 30
#define DEFAULT_DICT_SIZE 99172
char* dict_name = ""; 
int dictionary_size = 0; 
char dictionary[DEFAULT_DICT_SIZE][MAX_WORD_SIZE]; 


                        /**       Variables needed for connection
                        Includes the default port a client will use, and
                        the global variables of the port used **/     

#define DEFAULT_PORT 7000
int port = 0; 
int new_socket, c; 
struct sockaddr_in server, client; 

                        /**    Variable needed for log file     **/ 
FILE* logFile;  

                        /**      Variables needed for threads to run
                         * Includes the structure that holds the 
                         * primitives needed for synchronization and 
                         * the threads themselves, plus the variables
                         * they are protecting.                    **/
#define BUFFER_MAX 3
#define NUM_WORKERS 3
pthread_t pool[NUM_WORKERS], logT;
struct syncPrimitivesJob {
    pthread_mutex_t lock; 
    pthread_cond_t empty;
    pthread_cond_t fill;
    int buffer[BUFFER_MAX];
    int fill_ptr; 
    int use_ptr;
    int count;
};

struct syncPrimitivesLog {
    pthread_mutex_t lock; 
    pthread_cond_t empty;
    pthread_cond_t fill;
    char buffer[BUFFER_MAX][MAX_PHRASE_SIZE];
    int fill_ptr; 
    int use_ptr;
    int count;
};

struct syncPrimitivesJob socketInfo; 
struct syncPrimitivesLog logInfo; 

                        /**      Functions needed for
                         *  the buffers, both get and
                         *  put.                     **/

void addToSocketQueue(int value) {
    printf("TEST: Trying to acquire lock to add socket to queue...\n");
    pthread_mutex_lock(&socketInfo.lock);//Acquire lock
    printf("TEST: Lock acquired for adding a socket from queue!\n");
    while(socketInfo.count == BUFFER_MAX) {
        printf("TEST: Sitting here waiting for a space in the socket queue to empty out!\n");
        pthread_cond_wait(&socketInfo.empty, &socketInfo.lock);//Block thread while buffer is full
    }

    socketInfo.buffer[socketInfo.fill_ptr] = value;                         // This is process of
    socketInfo.fill_ptr = (socketInfo.fill_ptr + 1) % BUFFER_MAX;           // adding a socket to the
    socketInfo.count = socketInfo.count +1;                                 // queue 

    printf("TEST: I added something to the socket queue! A client has connected! Giving %d this index: %d.\n", value, socketInfo.count);
    pthread_mutex_unlock(&socketInfo.lock); //Release lock
    printf("TEST: Socket added and releasing lock!\n");
    pthread_cond_signal(&socketInfo.fill); //Signal that socket was added to queue
}

int removeFromSocketQueue() {
    printf("TEST: Trying to acquire lock to remove to queue...\n");
    pthread_mutex_lock(&socketInfo.lock); //Acquire lock
    printf("TEST: Lock acquired for removing a socket from queue!\n");

    while(socketInfo.count == 0) {
        printf("TEST: Sitting here waiting for socket to fill up!\n");
        pthread_cond_wait(&socketInfo.fill, &socketInfo.lock);//Block thread until buffer has item
    }

    int temp = socketInfo.buffer[socketInfo.use_ptr];               // This is process of
    socketInfo.use_ptr = (socketInfo.use_ptr +1) % BUFFER_MAX;      // removing a socket from
    socketInfo.count = socketInfo.count -1;                         // the queue

    printf("TEST: There's another space here in the socket buffer!\n");
    pthread_mutex_unlock(&socketInfo.lock); //Release lock 
    pthread_cond_signal(&socketInfo.empty); //Signal space is available in buffer
    return temp; //Return socket
}

void addToLogQueue(char phrase[MAX_WORD_SIZE]) {

    pthread_mutex_lock(&logInfo.lock); //Acquire lock

    while(logInfo.count == BUFFER_MAX) {
        pthread_cond_wait(&logInfo.empty, &logInfo.lock); //Block thread until queue has space
    }

    strcpy(logInfo.buffer[logInfo.fill_ptr], phrase);                    //This is the process of
    logInfo.fill_ptr = (logInfo.fill_ptr +1) % BUFFER_MAX;        //adding a phrase to 
    logInfo.count = logInfo.count +1;                             //the logInfo queue

    pthread_mutex_unlock(&logInfo.lock); //Release lock
    pthread_cond_signal(&logInfo.fill); //Signal logInfo was added to queue

}

char *removeFromLogQueue() {

    pthread_mutex_lock(&logInfo.lock); //Acquire lock

    while(logInfo.count == 0) {
        pthread_cond_wait(&logInfo.fill, &logInfo.lock);  //If logInfo queue is empty, wait to be signaled that it is full
    }

    char *temp; 
    temp = malloc(MAX_WORD_SIZE+1);
    temp = logInfo.buffer[logInfo.use_ptr];                   // This is the process of
    logInfo.use_ptr = (logInfo.use_ptr +1) % BUFFER_MAX;      // removing a socket from
    logInfo.count = logInfo.count -1;                         // the logInfo queue

    
    pthread_mutex_unlock(&logInfo.lock); //Release lock

    pthread_cond_signal(&logInfo.empty); //Signal spot is avaiable in buffer
    return temp; 
}
                                                                            /********************************************************
                                                                             ******************************************************** 
                                                                                The Dictionary Checking Function
                                                                                The dictionary is stored in an array, therefore
                                                                                we do a linear search on the array to see if the
                                                                                word is contained in this dictionary.
                                                                            *********************************************************
                                                                            *********************************************************/


int check_dict(char* word, char dictionary[dictionary_size][MAX_WORD_SIZE]) {
    //linear search for word
    for (int i = 0; word[i] != '\0'; i++) {
        while (!((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z') || word[i] == '\0')) {
            for (int j = i; word[j] != '\0'; j++) {
                word[j] = word[j + 1];
            }
            word[i] = '\0';
        }
    }
    char append = '\n';
    char *check = malloc(sizeof(word));
    strcpy(check, word); 
    strncat(check, &append, 1);
    printf("TEST: Checking for %s in dictionary.\n", word);
    for(int i = 0; i < dictionary_size-1; i++) {
        if(strcmp(check, dictionary[i]) == 0) {
            printf("%s OK\n", word);            //if the word is found, print OK
            free(check);
            return 0;
        }
    }
    printf("%s MISPELLED\n", word);             //if the word is not found, print mispelled
    free(check); 
    return -1; 
}

                                                                            /********************************************************
                                                                             ******************************************************** 
                                                                                            WORKER THREAD FUNCTION STARTS HERE
                                                                            The worker threads remove a socket descriptor from 
                                                                            connection queue, read a word from the socket, 
                                                                            check if the given word is in the dictionary, if it is found
                                                                            they write the word to the socket + "OK", if not they write
                                                                            the word + "MISPELLED". The word and result are sent as
                                                                            string to the logInfo queue. Continues til client disconnects. 
                                                                            *********************************************************
                                                                            *********************************************************/
//Steps: Create loop for continuous execution. Hold the lock for socket buffer. 
//       Wait using CV if buffer is empty. While not empty, remove socket descriptor.
//       Notify empty spot in queue (if waiters exist). Release lock. Service client.
//       Close socket.

//Steps to service client: While word is there to read, read word from socket. Check 
//                         dictionary. If exists, echo word to socket with "OK". If not,
//                         do same with "MISPELLED". Write the word and socket to logInfo queue. 
void* worker_threads(void* arg) { 
    while(1) { 
        int socket_desc; 
        printf("TEST: Worker thread activated. Sitting here waiting to be used!\n");
        while(socketInfo.count == 0) {
            continue;
        }
        socket_desc = removeFromSocketQueue();

        char message[] = "Connection to the server is successful. Enter a word for spell check or enter escape key to exit.\n";
        write(socket_desc, message, strlen(message));

        printf("TEST: Worker thread activated. Time to get a word!\n");

        char *word = calloc(MAX_WORD_SIZE, 1);
        while(read(socket_desc, word, MAX_WORD_SIZE) > 0) {
 
            if(word[0] == 27) {                     //If escape key is entered
                printf("Exiting %d client...\n", socket_desc);
                char goodbye[] = "Have a nice day!\n";                    
                write(socket_desc, goodbye, strlen(goodbye));
                break; 
            }

            if((strlen(word) <= 1)) {            //If nothing is entered
                continue;
            }

            int found = check_dict(word, dictionary);  //If something is entered, see if it is found
            if(found == 0) {
                word = realloc(word, sizeof(char*)*MAX_PHRASE_SIZE);
                strcat(word, " OK\n");
            } else {
                word = realloc(word, sizeof(char*)*MAX_PHRASE_SIZE);
                strcat(word, " MISPELLED\n");
            }
            write(socket_desc, word, strlen(word));
            addToLogQueue(word);
            word = calloc(MAX_WORD_SIZE, 1);
            char reprompt[] = "Please enter another word or press escape to exit.\n";
            write(socket_desc, reprompt, strlen(reprompt));
        }
        close(socket_desc);
    }
}

                                                                            /********************************************************
                                                                             ******************************************************** 
                                                                                            logInfo THREAD FUNCTION STARTS HERE
                                                                            The logInfo thread removes a string from logInfo buffer, writes to
                                                                            the logInfo file, all while providing mutual exclusion and 
                                                                            synchronization. 
                                                                            *********************************************************
                                                                            *********************************************************/

//Steps for logInfo: Create continuous loop for execution. While logInfo queue is not
//               remove entry from the logInfo and write the entry to the logInfo file. 
void* log_thread(void* arg) {
    char* response; 
    while(1) {
        printf("TEST: Log thread activated. We are writing to log!\n");
        response = removeFromLogQueue();  //Since the queue is no longer empty, then remove a socket descriptor for use
        printf("TEST: Pulled following from log queue: %s.\n", response);
        logFile = fopen("log.txt", "a");
        if(logFile == NULL) {
            printf("Error creating log file.");
            exit(1);
        } 
        fputs(response, logFile);
        fclose(logFile);
    }
}



                                                                            /********************************************************
                                                                             ******************************************************** 
                                                                                        MAIN FUNCTION STARTS HERE
                                                                                The main function can take in three arguments from
                                                                                the shell. If the user inputs a dictionary with a 
                                                                                .txt ending, it will use that dictionary. If the
                                                                                user inputs a port number, it will use that port. 
                                                                            *********************************************************
                                                                            *********************************************************/

//Steps: Allow for user input of port or dictionary. Load the dictionary (by input). 
//       Create the threads that will be running. Initialize the locks and condition
//       variables. 
//Steps for socket: Create continuous loop for execution. While true, connect socket
//                  by accepting listening socket. Add connected socket to wrok queue.
//                  Signal sleeping workers (if existent) that new socket is in queue.
int main(int argc, char** argv) {
                                /**********************************
                                 *  Process of checking if the user
                                 * has input any arguments, whether
                                 * a port or a .txt file. Initializes
                                 * dictionary and port at this point
                                 * *********************************/          

    //No port or dictionary name, load defaults
    if(argc == 1) {
        dict_name = DEFAULT_DICTIONARY; 
        port = DEFAULT_PORT;
    } else if (argc == 2) { //one input, could be dictionary name or port number
        if(strstr(argv[1], ".txt") == NULL) {
            port = atoi(argv[1]);
            dict_name = DEFAULT_DICTIONARY;
        }
        if(strstr(argv[1], "txt") != NULL) {
            port = DEFAULT_PORT;
            dict_name = argv[1];
        }
        printf("Based on your two inputs, the port is %d and the dictionary name is %s.\n", port, dict_name);
    } else if (argc == 3) { //two inputs, should be both dictionary name and port number
        if((strstr(argv[1], ".txt") != NULL) && (strstr(argv[2], ".txt") == NULL)) { //first case, port then dictionary
            dict_name = argv[1];
            port = atoi(argv[2]);
        } else if((strstr(argv[1], ".txt") != NULL) && (strstr(argv[2], ".txt") == NULL)) { //second case, dictionary name then prt
            dict_name = argv[2];
            port = atoi(argv[1]);
        } else {
            printf("Input cannot be verified as port and dictionary name. Please enter correct arguments.\n");
            exit(1);
        }
        printf("Based on your three inputs, the port is %d and the dictionary name is %s.\n", port, dict_name);
    }
    else {
        printf("You have entered too many arguments. Please try again.\n");
        exit(1);
    }

    printf("TEST: Based on your specifications, port used is %d and dictionary name is %s.\n", port, dict_name);

                                    /*****************************
                                    * Loading the dictionary based 
                                    * off of the input we have 
                                    ****************************/

    FILE *dictionaryFile = fopen(dict_name, "r");
    if(dictionaryFile == NULL) {
        printf("Cannot open log file.\n");
        exit(1);
    }
    
    
    //Check for amount of space needed for dictionary array
    char ch; 
    int dict_size = 0; 
    do {
        ch = fgetc(dictionaryFile);
        if(ch == '\n') 
        dict_size++;
    } while (ch != EOF);
    rewind(dictionaryFile);
    dictionary_size = dict_size+1; //+1 because it will not count last word

    //Read in each line and set word to space in dictionary array
    int count = 0; 
    while(count < dictionary_size && (fgets(dictionary[count], sizeof(dictionary[count]), dictionaryFile) != NULL)) {
        count++; 
    }
    printf("TEST: We have %d words in the dictionary.\n", dict_size); 

    //Close the file
    fclose(dictionaryFile); 

                                /**********************************
                                 *  Creation of threads that will
                                 * be running, both workers and 
                                 * log. Also creation of log
                                 * file. 
                                 * *********************************/ 

    for(int i = 0; i < NUM_WORKERS; i++) {
        if(pthread_create(&pool[i], NULL, worker_threads, NULL) != 0) {
            printf("Worker thread %d failed to create.", i);
            exit(1);
        }
    }
    if(pthread_create(&logT, NULL, log_thread, NULL) != 0) {
        printf("Log thread failed to create.");
        exit(1);
    }

    logFile = fopen("log.txt", "a");
    if(logFile == NULL) {
        printf("Error creating log file.");
        exit(1);
    }
    fclose(logFile);

    printf("TEST: Worker threads and log thread is created. Log file has been created.\n");


                                /**********************************
                                 *  Initalizing locks, condition
                                 * variables and buffers. 
                                 * *********************************/ 

    if(pthread_mutex_init(&socketInfo.lock, NULL) != 0) {
        printf("Initializing socketInfo mutex lock failed.");
        exit(1);
    }
    if(pthread_mutex_init(&logInfo.lock, NULL) != 0) {
        printf("Initializing logInfo mutex lock failed.");
        exit(1);
    }
    if(pthread_cond_init(&socketInfo.fill, NULL) != 0) {
        printf("Initializing socketInfo fill CV failed.");
        exit(1);
    }
    if(pthread_cond_init(&socketInfo.empty, NULL) != 0) {
        printf("Initializing socketInfo empty CV failed.");
        exit(1);
    }
    if(pthread_cond_init(&logInfo.fill, NULL) != 0) {
        printf("Initializing logInfo fill CV failed.");
        exit(1);
    }
    if(pthread_cond_init(&logInfo.empty, NULL) != 0) {
        printf("Initializing logInfo empty CV failed.");
        exit(1);
    }

    socketInfo.fill_ptr = 0;
    socketInfo.use_ptr = 0;
    socketInfo.count = 0;

    logInfo.fill_ptr = 0;
    logInfo.use_ptr = 0;
    logInfo.count = 0;

    printf("TEST: Initialization of all variables has occurred.\n");


                                    /*****************************
                                    * Creating active socket 
                                    * descriptor, preparing
                                    * sockaddr_insructure, 
                                    * binding the socket address
                                    * to the socket descriptor,
                                    * and listening for incoming 
                                    * connections. 
                                    ****************************/ 
                                
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);          //Creating active socket descriptor
    if(socket_desc == -1) {
        printf("Error creating socket!");
        exit(1);
    } 
    printf("TEST: Active socket descriptor is created.\n");

    server.sin_family = AF_INET;                            //Preparing sockaddr_instructure
    server.sin_addr.s_addr = INADDR_ANY; //defaults to 127.0.0.1, loopback server
    server.sin_port = htons(port);

    printf("TEST: Sockaddr_instructure is created.\n");

    int bind_result = bind(socket_desc, (struct sockaddr*)&server, sizeof(server)); //Binding
    if(bind_result < 0) {
        printf("Error in Bind.");
        exit(1);
    }

    printf("TEST: Binding is complete.\n");

    if(listen(socket_desc, 3) == 0) {
        printf("TEST: Listening...\n");
    }

                                    /*****************************
                                    * Accepting and distributing 
                                    * connection requests. 
                                    ****************************/   
    while(1) { 
        printf("TEST: Now waiting for connection requests.\n");
        c = sizeof(struct sockaddr_in);
        new_socket = accept(socket_desc, (struct sockaddr *) &client, (socklen_t*) &c);
        if(new_socket < 0) {
            printf("Error connecting. Please try again.");
            continue;
        } 
        printf("Connection accepted!\n");
        addToSocketQueue(new_socket);
    }
}
