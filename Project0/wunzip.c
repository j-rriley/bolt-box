#include <stdio.h>
#include <stdlib.h>

//wcat program

int main(int argc, char **argv){
    char *input;
    
    //if passed no command-line arguments
    if(argc == 1) {
        printf("wunzip: file1 [file2 ...]\n");
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
            printf("wunzip: cannot open file\n");
            exit(1);
        }

        size_t size = 0; 
        ssize_t read;
        char *line = NULL; 
        char *new=NULL;
        //read the lines
        while((read = (getline(&line, &size, fp))) != -1){
            int i = 0; 
            while(i <= read) {
                if(read - i > 5) {
                    for(i = 0; i < 5; i++) {
                    new[0] = (int)line[i+0];
                    new[1] = (int)line[i+1]; 
                    new[2] = (int)line[i+2];
                    new[3] = (int)line[i+3];
                    new[4] = line[i+4];
                    }
                fwrite(&line, 5, 1, stdout);
                i = i+5;
                }
            }
        }
        counter = counter + 1; 
    }
    exit(0);
}
