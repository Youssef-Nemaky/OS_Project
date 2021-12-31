#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This is not int main(VOID) as we are passing arugments to our main function int main(argc, char * argv[]) 
 * argc (arguments counter): number of passed arguments to the terminal 
 * EX: in terminal ( ./wcat file.txt ) 
 * number of arguemnts is 2 wcat (name of the program you want to run) and the other arguments like file.txt
 * argv[0] is wcat
 * argv[1] is file.txt 
 */
int main(int argc, char *argv[]){
    /* A pointer to point to the opened file */
    FILE * filePtr = NULL;
    /* A pointer to point to a created temporary file */
    FILE * tempFilePtr = NULL;
    /* A pointer that points to the dynamically allocated array of chracters returned by getline */
    char * linePtr = NULL;
    /* Indexing pointers */
    char * start = NULL;
    char * end = NULL;
    /* Number of chracters found in a row */
    int runLength = 1;
    /* A flag to check whether the new line has been printed or not */
    int newLineFlag = 0;
    /* A counter to hold number of new lines found in a row */
    int newLineCounter = 1;
    /* Size of the returned array of chracters from getline */
    size_t lineSize = 0;
    /* number of elements returned by getline */
    int numberOfChracters;

    /* Checking if no arugments were passed */
    if(argc < 2){
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    } 

    /* Make a temporary file and concatinate all files in it */
    /* Make the file and if it does not exit and append to the end of the file. */
    tempFilePtr = fopen("wzip_temp.txt", "a+"); 
    
    /* Loop through the files one by line */
    for(int i = 0; i < argc - 1; i++){
        /* Open the file at argv[i + 1] */
        filePtr = fopen(argv[i + 1], "r");
        
        /* Error checking to make sure the file was opened successfully */
        if(filePtr == NULL){
            printf("wzip: cannon open file\n");
            return 1;
        }
        /* Loop through the file and read all lines and type write them in the temporary file */
        while((numberOfChracters = getline(&linePtr, &lineSize, filePtr)) != -1){
            /* write the read line to the temporary file */
            fwrite(linePtr,sizeof(char), numberOfChracters, tempFilePtr);    
        }
        /* close the file */
        fclose(filePtr);
    }
    /* Move the pointer of the file to the begining */
    rewind(tempFilePtr);    
    
    /* loop through the temporary file (it now contains all the read files in it) and compress it */
    while(getline(&linePtr, &lineSize, tempFilePtr) != -1){
        start = linePtr;
        end = linePtr + 1;
        if(linePtr[0] == '\n'){
            newLineCounter++;
        } else {
            /* print new lines */
            if(newLineFlag == 1){
                newLineFlag = 0;
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
            newLineFlag = 1;
        }
    }
    if(newLineFlag == 1){
        newLineFlag = 0;
        fwrite(&newLineCounter, 1, sizeof(int), stdout);
        fwrite("\n", sizeof(char), 1, stdout);
        newLineCounter = 1;
    }
    /* close the temporary file */
    fclose(tempFilePtr);
    /* remove the temporary file */
    remove("wzip_temp.txt");
    /* Free the dynamically allocated returned array of chracters returned by getline */
    free(linePtr);
    return 0;
}