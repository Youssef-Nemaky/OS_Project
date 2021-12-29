#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    char * linePtr = NULL;
    FILE * filePtr = NULL;
    size_t bufferSize = 0;
    char * searchResult = NULL;

    if(argc < 2){
        printf("wgrep: searchterm [file ...]\n");
        return 1;
    } else if (argc < 3) {
        /* Read from STDIN */
    } else {
        /* Read from the files */
        filePtr = fopen(argv[2], "r");

        if(filePtr == NULL){
            printf("wgrep: cannot open file\n");
            return 1;
        }

        getline(&linePtr,&bufferSize,filePtr);

        if(linePtr == NULL){
            printf("line reading failed\n");
            return 1;
        }

        printf("%s\n", linePtr);

        searchResult = strstr(linePtr,argv[1]);

        if(searchResult != NULL){
            printf("%s was found in %s\n", argv[1], searchResult);
            //printf("%s",linePtr);
        } else {
            printf("Couldn't find %s in the first line\n", argv[1]);
        }
        free(linePtr);
        fclose(filePtr);
    }
    
    
    return 0;
}