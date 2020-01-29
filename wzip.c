#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
    char *input;
    
    //if passed no command-line arguments
    if(argc == 1) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }
    
    //setting up for potential of multiple files
    int counter = 1; 

    while(counter < (argc)) {
        //picking the file
        input = argv[counter];

        //open the file
        FILE *fp = fopen(input, "r");

        //if file does not work
        if(fp == NULL) {
            printf("wzip: cannot open file\n");
            exit(1);
        }

        size_t size = 0; 
        ssize_t read;
        char *line = NULL; 

        //read the lines
        while((read = (getline(&line, &size, fp))) != -1){
            for(int i = 0; i < read - 1; i++) {
                int count = 1; 
                while(line[i] == line[i+1]) {
                    i++;
                    count++; 
                    if (i+1 == read){
                        break;
                    }        
                    
                }
                fwrite(&count, 4, 1, stdout);
                fwrite(&line[i], 1, 1, stdout);
            }
        }
        counter = counter + 1; 
    }
    exit(0);
}