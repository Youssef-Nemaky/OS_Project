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
        filePtr = stdin;
    } else {
        /* Read from the files */
        filePtr = fopen(argv[2], "r");
    
        if(filePtr == NULL){
            printf("wgrep: cannot open file\n");
            return 1;
        }
    }

    while (getline(&linePtr, &bufferSize, filePtr) != -1){
        searchResult = strstr(linePtr, argv[1]);
        if (searchResult != NULL)
        {
            printf("%s", linePtr);
        }
    }
    free(linePtr);
    fclose(filePtr);

    return 0;
}