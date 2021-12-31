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
    /* linePtr: A pointer to point to the returned malloced chracters array */
    char * linePtr = NULL;
    /* filePtr: A pointer to point to the opened file */
    FILE * filePtr = NULL;
    /* bufferSize: Hold the size of the returned malloced chracters array */
    size_t bufferSize = 0;
    /* returns a pointer when searching for a string succeeds, else it returns NULL */
    char * searchResult = NULL;
    /* A variable to hold the number of files opened */
    int numberOfFiles = 0;
    
    /* Checking if no arugments were passed */
    if(argc < 2){
        printf("wgrep: searchterm [file ...]\n");
        return 1;
    } else if(argc < 3){
        /* Checking if you should read from the stdin */
        /* stdin is already an opened file when the program runs (ALSO: stdout and error files) */
        filePtr = stdin;
        numberOfFiles = 1;
    } else {
        /* Checking if you should read from files other than stdin */
        numberOfFiles = argc - 2;
        /* Open the first file */
        filePtr = fopen(argv[2], "r");
    }

    /* Loop through the files one by one */
    for(int i = 0;i < numberOfFiles;i++){
        /* Check if it is not the first file to open and not the stdin file */
        if(numberOfFiles != 1){
            filePtr = fopen(argv[i + 2], "r");
        }

        /* Error checking to make sure the file was opened successfully */
        if(filePtr == NULL){
            printf("wgrep: cannot open file\n");
            return 1;
        }
        /* getline() is a function to read a line from a file malloc an array of chracters for it
         * and return the address of the first element and the buffer size.
         * 1st argument: A pointer to point to the returned array of characters
         * 2nd argument: Returned buffer size of the array of chracters
         * 3nd argument: The file to read from
        */
        
        /* Loop through the file line by line */
        while (getline(&linePtr, &bufferSize, filePtr) != -1){
            /* Search for the string you are looking for in the line you just read */
            searchResult = strstr(linePtr, argv[1]); /* you should include string.h to use strstr() */
            /* If you found a string that matches the string you are looking for then print the line */
            if (searchResult != NULL){
                printf("%s", linePtr);
            }
        }
        /* close the file */
        fclose(filePtr);
    }
    /* Free the dynamically allocated returned array of chracters returned by getline */
    free(linePtr);

    return 0;
}