#include <stdio.h>
#include <stdlib.h>


int main(int argc, char * argv[]){
    FILE * filePtr;
    char buffer[3];

    if(argc < 2){
        return 0;
    } 

    /* second edition: printing more than one file */

    for(int i = 0; i < argc - 1; i++){
        filePtr = fopen(argv[i + 1], "r");
        
        if (filePtr == NULL){
            printf("wcat: cannot open file\n");
            return 1;
        }

        while (fgets(buffer, 2, filePtr) != NULL){
            printf("%s", buffer);
        }

        fclose(filePtr);
    }

    return 0;
}