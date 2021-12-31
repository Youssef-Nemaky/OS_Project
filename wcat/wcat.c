#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 2048


/* This is not int main(VOID) as we are passing arugments to our main function int main(argc, char * argv[]) 
 * argc (arguments counter): number of passed arguments to the terminal 
 * EX: in terminal ( ./wcat file.txt ) 
 * number of arguemnts is 2 wcat (name of the program you want to run) and the other arguments like file.txt
 * argv[0] is wcat
 * argv[1] is file.txt 
 */
int main(int argc, char * argv[]){
    /* A pointer to point at the file opened */
    FILE * filePtr;
    /* An array to hold the read characters from the lind */
    char buffer[BUFFER_SIZE];

    /* Checking if no arguments were passed */
    if(argc < 2){
        return 0;
    } 

    /* Looping through the files one by one */
    for(int i = 0; i < argc - 1; i++){
        /* open the file at arguemnt i + 1 
         * fopen returns NULL if fopen failed to open the file
         */
        filePtr = fopen(argv[i + 1], "r"); /* Why i + 1 ? Check the comment before main function :) */ 
        
        /* Error checking to check if fopen failed to open the file */
        if (filePtr == NULL){
            printf("wcat: cannot open file\n");
            return 1;
        }

        /* Read the 2048 (BUFFER_SIZE) characters and store them in buffer 
         * if fgets reaches the EOF (END OF FILE), it returns NULL
         */
        while (fgets(buffer, BUFFER_SIZE, filePtr) != NULL){
            printf("%s", buffer);
        }
        
        /* close the opened file */
        fclose(filePtr);
    }

    return 0;
}