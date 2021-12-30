#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE * filePtr = NULL;
    char * linePtr = NULL;
    char * start = NULL;
    char * end = NULL;
    int runLength = 1;
    int firstLine = 1;
    size_t lineSize = 0;

    if(argc < 2){
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    } 

    for (int i = 0; i < argc - 1; i++){
        firstLine = 1;
        filePtr = fopen(argv[i + 1], "r");
        if (filePtr == NULL){
            printf("wzip: cannot open file\n");
            return 1;
        }

        while (getline(&linePtr, &lineSize, filePtr) != -1){

            /* now linePtr is like an array of string containing the line which fgets read */
            start = linePtr;
            end = linePtr + 1;

            while ((*start != '\0') && (*start != '\n') && (*start != '\r') && (*start != ' ')){
                if(firstLine == 1){
                    firstLine = 0;
                } else {
                    fwrite("\n", sizeof(char), sizeof(char), stdout);
                }
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
        }
        free(linePtr);
        fclose(filePtr);
    }

    return 0;
}