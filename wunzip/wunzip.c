#include <stdio.h>
#include <stdlib.h>

/* This is not int main(VOID) as we are passing arugments to our main function int main(argc, char * argv[]) 
 * argc (arguments counter): number of passed arguments to the terminal 
 * EX: in terminal ( ./wcat file.txt ) 
 * number of arguemnts is 2 wcat (name of the program you want to run) and the other arguments like file.txt
 * argv[0] is wcat
 * argv[1] is file.txt 
 */
int main(int argc, char * argv[]){
    /* A pointer to point to the opened file */
    FILE * filePtr = NULL;
    /* numberOfChrars variable to hold the run-length */
    int numberOfChars = 0;
    /* Chracter: A variable to hold the chracter after the run-length */
    char character = 0;

    /* Checking if no arugments were passed */
    if(argc < 2){
        printf("wunzip: file1 [file2 ...]\n");
        return 1;
    }

    /* Loop through the files one by one and de-compress them */
    for(int i = 0; i < argc - 1; i++){
        /* Open the file at argv[i + 1] */
        filePtr = fopen(argv[i + 1],"r");

        /* Error checking to make sure the file was opened successfully */
        if(filePtr == NULL){
            printf("wzip: cannon open file\n");
            return 1;
        }
        
        /* Keep reading the file until you reach the end of it */ 
        while((fread(&numberOfChars, sizeof(int), 1, filePtr) != 0) && (fread(&character, sizeof(char), 1, filePtr) != 0))
        {   
            /* Print the character numberOfChars times */
            while (numberOfChars > 0){
                printf("%c", character);
                numberOfChars--;
            }
        }
        /* Close the file */
        fclose(filePtr);
    }
    return 0;
}