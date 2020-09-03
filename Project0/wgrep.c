#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
    char *input;

    //if passed no command-line arguments
    if(argc == 1) {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }

    
    char *phrase = argv[1];
    int counter = 2; 

    while(counter < argc || argc == 2) {
        //picking the file
        input = argv[counter];

        //open the file
        FILE *fp = fopen(input, "r");
        
        //if file is invalid

        //if no file is input but a search term is
         if(argc == 2 && input == NULL) {
             fp = stdin; 
        }

        if(fp == NULL && argc != 2) {
            printf("wgrep: cannot open file\n");
            exit(1);
        }

        size_t size = 0; 
        ssize_t read;
        char *line = NULL; 
        
        //read the lines
        while((read = (getline(&line, &size, fp))) != -1){
            if(strstr(line, phrase) != NULL) {
                printf("%s", line);
            }
    }
    counter = counter + 1;
	argc = 0;  

    }
exit(0);
}
