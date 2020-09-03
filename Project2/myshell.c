/********************************************** Jaime Riley ********************************************
 * 												 tuh06971
 * 											  Project 2: Shell 		
 * 												Main program		
 *******************************************************************************************************/





#include "myshell.h"

/********************************************************************************************************************************
****************************************************BUILT IN FUNCTIONS***********************************************************
*********************************************************************************************************************************/


//CD
//changes directory based off of input
void we_cd(char** tokens) {
	//if no specified directory, print current directory 
	if(!tokens[1] || strcmp(tokens[1], " ") == 0) {
		char* cwd;
		cwd = getcwd(cwd, sizeof(cwd)); 
		printf("%s\n", cwd); 
	}
	//otherwise, change to specified directory
	if(tokens[1]) {
		chdir(tokens[1]);
	}
	else {
		write(STDERR_FILENO, error_message, strlen(error_message));
	}
}

//CLEAR
//removes all previous commands and output from consoles and terminal windows 
void we_clr(char** tokens) {
	printf("\033[H\033[J");
}


//LIST DIRECTORY
//lists the contents of current directory
void we_dir(char** tokens) {
	struct dirent *direct; 
	DIR *d = opendir("."); 
	if(d == NULL) {
		write(STDERR_FILENO, error_message, strlen(error_message));
	}
	else {
		while ((direct = readdir(d)) != NULL) {
			printf("%s ", direct->d_name); 
		}
	}
	closedir(d); 
}


//LIST ENVIRONMENT
//list environment strings
void we_environ(void) {
	int i = 0; 
	while(environment[i] != NULL) {
		printf("%s\n", environment[i]); 
		i++;
	}
}


//DISPLAYING COMMENT
//displays line of text/string that is passed as an argument followed by new line
void we_echo(char** tokens) {
	int i = 1; 
	while(tokens[i] != NULL) {
		if(strcmp(tokens[i], ">") == 0) {
			break; 
		}
		printf("%s\n", tokens[i]); 
		i++; 
	}
}


//HELP LIST
//lists all possible commands and uses
void we_help(char** tokens) {
	printf("List of commands usable in this shell: \n");
    printf("Remove command_ to utilize. \n"); 
	printf("command_cd: changes the directory to input that follows. if directory is not specified, \n");
	printf("it will tell you the current directory.\n");
	printf("command_clr: this will clear the screen. \n");
	printf("command_dir: this will list the contents of the directory you are in.\n");
	printf("command_environ: this will list the environment strings. \n");
	printf("write more to hear more...\n"); 

	if(strcmp(tokens[2], "more")) {
		printf("command_echo will display the desired argument on the display followed by a new line\n");
		printf("command_help shows the options of commands.\n");
		printf("command_pause will allow a user to stop operations until the enter key is pressed.\n");
		printf("command_quit will exit the shell.\n");
	}
}


//PAUSING
//pause until user presses enter
void we_pause() {
    //pretty self explanatory, if it is not an enter key, we will continuously run in this loop
	while(getchar() != '\n') {
		continue; 
	}
}

//QUITTING THE SHELL
void we_quit() {
    //also pretty self explanatory, if called the shell will exit
	exit(0);
}

/*******************************************************************************************************
**********************************functions that will occur in the main loop****************************
********************************************************************************************************/


//this is simply for my own satisfaction, creating a starting point to the shell.
void start_shell() {
    printf("begin shell:\n");
}

//TO BEGIN: The function that will allow the shell to get and store input
//function that gets user input one line at a time from standard input. returns a character array that consists of the line input
char* get_input(char* input_type) {
    char* line; 
	//if the input is not empty, read from there
	if(input_type != NULL) {
		FILE* input = fopen(input_type, "r");
		if(input == NULL) {
			write(STDERR_FILENO, error_message, strlen(error_message));
			exit(1); 
		}
 		size_t length = 1024;
		line = fgets(line, length, input);
	}  
	//if the input is empty, read from standard input
	else {
 		size_t length = 1024;
		line = fgets(line, length, stdin);
	}
	return line; 
}

//NEXT: The function that divides up the input
//tokenizer that returns user input as white-space delimited tokens and creating an array from it
char** create_token(char* input) {
    char* temp; 
	char** tokens = malloc(sizeof(char **));
	temp = strtok(input, " \t\r\n\a"); 
	int i = 0; 
    while(temp != NULL) {
		tokens[i] = temp;
        i++;  
	}

    temp = strtok(NULL, " \t\r\n\a");
	return tokens; 
}


//NEXT: The function that simplifies the input into commands 
//creates the processes that run
int define_commands(char** tokens) {
	int i = 1; 
    while(tokens[i]) {
        if(strcmp(tokens[i], "cd")) {
            *tokens[i] = cd; 
        } 
        if(strcmp(tokens[i], "clr")) {
            tokens[i] = clr; 
        }
        if(strcmp(tokens[i], "dir")) {
            tokens[i] = dir; 
        }
        if(strcmp(tokens[i], "environ")) {
            tokens[i] = environ; 
        }
        if(strcmp(tokens[i], "echo")) {
            tokens[i] = echo; 
        }
        if(strcmp(tokens[i], "pause")) {
            tokens[i] = pause; 
        }
        if(strcmp(tokens[i], "help")) {
            tokens[i] = help; 
        }
        if(strcmp(tokens[i], "quit")) {
            tokens[i] = quit; 
        }
        if(strcmp(tokens[i], "<")) {
            tokens[i] = redirect_in; 
        }
        if(strcmp(tokens[i], ">")) {
            tokens[i] = redirect_out; 
        }
        if(strcmp(tokens[i], ">>")) {
            tokens[i] = redirect_outA; 
        }
        if(strcmp(tokens[i], "|")) {
            tokens[i] = pipe; 
        }
        if(strcmp(tokens[i], "&")) {
            tokens[i] = multitask; 
        }
        //means it is a different argument, either filename or directory
        else {
            i++; 
            continue; 
        }
        i++; 
    }
}



void exec_commands(char** tokens) {
    int i = 1; 
	while(tokens[i]) {
        if(tokens[i] >= 1 && tokens[i] <= 13) {
            if(tokens[i] == 1) {
                we_cd(tokens); 
            }
            if(tokens[i] == 2) {
                we_clr(tokens); 
            }
            if(tokens[i] == 3) {
                we_dir(tokens); 
            }
            if(tokens[i] == 4) {
                we_environ(tokens); 
            }
            if(tokens[i] == 5) {
                we_echo(tokens); 
            }
            if(tokens[i] == 6) {
                we_pause(tokens); 
            }
            if(tokens[i] == 7) {
                we_help(tokens); 
            }
            if(tokens[i] == 8) {
                we_quit(tokens); 
            }
            if(tokens[i] == 9) {
                input_redirection(tokens[i+1]); 
            }
            if(tokens[i] == 10) {
                output_redirection(tokens, 11); 
            }
            if(tokens[i] == 11) {
                output_redirection(tokens, 12); 
            }
            if(tokens[i] == 12) {
                pipe_line(tokens); 
            }
            if(tokens[i] == 13) {
                multitask_func(tokens); 
            }
            else {
                i++;
                continue; 
            }
            i++;
        }
    }
}



/***************************************************************************************************************
************************************FUNCTIONS THAT ALLOW FOR REDIRECTIONS***************************************
****************************************************************************************************************/


//INPUT REDIRECTION
//function that is input redirection using < (getting input from what is following "<", change get_input function)
void input_redirection(char* new_input) {
	get_input(new_input); 
}


//OUTPUT REDIRECTION
//function that is output redirection using > (sending output to what is following ">")
FILE output_redirection(char** tokens, int type) {
	//first, open up the new file we are going to write to
    if(type == 11) {
        FILE *fp = fopen(new_output, "w");    
    }
    else {
	    FILE *fp = fopen(new_output, "a");
    }
        if(fp == NULL) {
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
	//return the file we are going to write to
	return fp; 
}




//PIPING A FUNCTION
//function that is pipe using |
void pipe_line(char** tokens) {
	//this function needs to take the output of one function and direct it to another function, therefore it will need to return
	//the output of the first function and in the main call we will need to declare the input of the second function as this function

}


//RUNNING PROCESSES SIMULTANEOUSLY
//function that is multiprocessing using &
void multitask_func(char** tokens) {
	//this function needs to allow for the two 
}




/***********************************************************************************EXECUTION*****************************************************************************/
//main, handles both batchfile or loop
int main() {
    start_shell(); 
    ///in the case of a batch file
	char* batch = NULL; 
    if(argv[1]) {
        batch = argv[1];
        FILE *fp = fopen(batch, "r");
        char buf[1024]; 
        char** tokens; 
        
        if(fp == NULL) {
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
        else {
        fgets(buf, 1024 * sizeof(char), fp); 
            while(!feof(fp)) {
                tokens = create_token(buf);
                define_commands(tokens);
                exec_commands(tokens); 
                fgets(buf, sizeof(buf), fp);
            }
            fclose(fp); 
        }
    }

    else {

    }
    //not a batchfile, stdin

}
