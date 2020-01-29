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

        char ch; 
        char check;
        int place; 
        int countCh;
        //read the lines
        while((read = (getline(&line, &size, fp))) != -1){
            place = 0; 
            countCh = 1;
            //we will always start by looking for the first character in the line as long as it isnt a space
            if(line[place] != ' '){
                ch = line[place];
            }
            else{
                while(line[place] == ' ') {
                    place++; 
                }
                ch = line[place];

                if(ch == '\n') {
                    continue; 
                }
            }

            //while we are not at the end of the line
            while(place <= read) {
                //check the character after 
                check = line[place+1]; 
                if(check == ch) {
                    countCh++; 
                    place++;  
                }
                else {
                    fputc((char)countCh, stdout);
                    fputs("   ", stdout); 
                    fputc(ch, stdout);
                    countCh = 0; 
                    place++; 

                    while(((check = line[place]) == ' ') || ((check = line[place])  == '\n')) {
                        if((check = line[place]) == ' ') {
                            place++; 
                        }
                        if((check = line[place]) == '\n') {
                            break; 
                    }
                } 
                ch = line[place];
                }
            }
        }
    counter = counter + 1;
    }
exit(0);
}