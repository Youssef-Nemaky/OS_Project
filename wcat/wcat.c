#include <stdio.h>
#include <stdlib.h>


int main(int argc, char * argv[]){
    FILE * filePtr;
    char buffer[3];
    if(argc < 2){
        printf("No input files specified!\n");
        return 1;
    } 

    /* first edition: printing only one file */

    filePtr = fopen(argv[1], "r");

    if(filePtr == NULL){
        printf("cannot open file\n");
        return 1;
    } 

    while(fgets(buffer,2,filePtr) != NULL){
        printf("%s",buffer);
    }

    fclose(filePtr);
    return 0;
}