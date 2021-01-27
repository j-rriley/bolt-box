#include <stdio.h>
#include <stdlib.h>

//wcat program

int main(int argc, char **argv){
    int counter = 1; 
    while(counter != argc) {
        char *input = argv[counter];
        FILE *fp = fopen(input, "r");
        if(fp == NULL) {
            printf("wcat: cannot open file\n");
            exit(1);
        }
        int s = fgetc(fp); 
        while(s != EOF) {
            printf("%c", s);
            s = fgetc(fp);
            }
        counter = counter+1; 
        fclose(fp);
    }
}
