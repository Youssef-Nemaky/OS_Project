#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE * filePtr = NULL;
    char * linePtr = NULL;
    char * start = NULL;
    char * end = NULL;
    int runLength = 1;
    size_t lineSize = 0;

    if(argc < 2){
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    } 

    filePtr = fopen(argv[1], "r");

    if(filePtr == NULL){
        printf("wzip: cannot open file\n");
        return 1;
    }

    while(getline(&linePtr,&lineSize,filePtr) != -1){

        /* now linePtr is like an array of string containing the line which fgets read */
        start = linePtr;
        end = linePtr + 1;

        while ((*start != '\n') && (*start != '\r') && (*start != ' ')){
            runLength = 1;
            while (*end == *start){
                runLength++;
                end++;
            }
            fwrite(&runLength, 1, sizeof(int), stdout);
            fwrite(start, 1, sizeof(char), stdout);
            start = end;
            end++;
        }
        fwrite("\n",sizeof(char),sizeof(char),stdout);
    }

    return 0;
}