#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    char * linePtr = NULL;
    FILE * filePtr = NULL;
    size_t bufferSize = 0;
    char * searchResult = NULL;
    int numberOfFiles = 0;
    
    if(argc < 2){
        printf("wgrep: searchterm [file ...]\n");
        return 1;
    } else if(argc < 3){
        filePtr = stdin;
        numberOfFiles = 1;
    } else {
        numberOfFiles = argc - 2;
        filePtr = fopen(argv[2], "r");
    }

    for(int i = 0;i < numberOfFiles;i++){
        if(numberOfFiles != 1){
            filePtr = fopen(argv[i + 2], "r");
        }

        if(filePtr == NULL){
            printf("wgrep: cannot open file\n");
            return 1;
        }

        while (getline(&linePtr, &bufferSize, filePtr) != -1){
            searchResult = strstr(linePtr, argv[1]);
            if (searchResult != NULL){
                printf("%s", linePtr);
            }
        }
        fclose(filePtr);
    }

    free(linePtr);

    return 0;
}