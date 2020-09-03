/********************************************** Jaime Riley ********************************************
 * 												 tuh06971
 * 											  Project 2: Shell 		
 * 												Include file		
 *******************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h> 
#include <dirent.h>

//defining functions that can be called
#define cd 1
#define clr 2
#define dir 3
#define environ 4
#define echo 5
#define pause 6
#define help 7
#define quit 8
#define redirect_in 9
#define redirect_out 10
#define redirect_outA 11
#define pipe 12
#define multitask 13

//showing what is currently happening in the shell
extern char **environment; 
char error_message[30] = "An error has occurred\n";

