#include <stdio.h>
#include <stlib.h>

int main(int argc, char **argv){
    //if passed no command-line arguments

    if(argc == 0) {
        printf("wgrep: searchterm [file...]\n");
        exit(1);
    }
    
    int counter = 2; 

    while(counter <= argc) {
        //picking the file
        char *input = argv[counter];

        //if no files were input
        if(input == NULL) {
            input = stdin; 
        }

        //open the file
        FILE *fp = fopen(input, "r");

        //if file is invalid
        if(fp == NULL) {
            printf("wgrep: cannot open file\n");
            exit(1);
        }

        //reading each line of the file for the input phrase 
        size_t length = 0; 
        ssize_t read;    
        //read the lines
        while((read = getline(&line, &length, fp)) != -1){
        

        }
    }
    exit(0);
}