#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE * filePtr = NULL;
    FILE * tempFilePtr = NULL;
    char * linePtr = NULL;
    char * start = NULL;
    char * end = NULL;
    int runLength = 1;
    int flag = 0;
    int newLineCounter = 1;
    size_t lineSize = 0;
    int x;

    if(argc < 2){
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    } 
    // I have to make a file that will take the cat result of all files

    tempFilePtr = fopen("wzip_temp.txt", "a+"); 
    
    for(int i = 0; i < argc - 1; i++){
        filePtr = fopen(argv[i + 1], "r");
        //Make file and if does not exit, create one and append to the end of the file.

        if(filePtr == NULL){
            printf("wzip: cannon open file\n");
            return 1;
        }

        while((x = getline(&linePtr, &lineSize, filePtr)) != -1){
            fwrite(linePtr,sizeof(char), x, tempFilePtr);    
        }
        fclose(filePtr);
    }

    rewind(tempFilePtr);    
    
    /* loop through the file and compress it */

    while(getline(&linePtr, &lineSize, tempFilePtr) != -1){
        start = linePtr;
        end = linePtr + 1;
        if(linePtr[0] == '\n'){
            newLineCounter++;
        } else {
            /* print new lines */
            if(flag == 1){
                flag = 0;
                fwrite(&newLineCounter, 1, sizeof(int), stdout);
                fwrite("\n", sizeof(char), 1, stdout);
                newLineCounter = 1;
            }
        }
        while ((*start != '\0') && (*start != '\n')){
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
        if(*start == '\n'){
            flag = 1;
        }
    }
    if(flag == 1){
        flag = 0;
        fwrite(&newLineCounter, 1, sizeof(int), stdout);
        fwrite("\n", sizeof(char), 1, stdout);
        newLineCounter = 1;
    }
    fclose(tempFilePtr);
    remove("wzip_temp.txt");
    free(linePtr);
    return 0;
}