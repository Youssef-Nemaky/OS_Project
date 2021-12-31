#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
    FILE * filePtr = NULL;
    int numberOfChars = 0;
    
    char character = 0;

    if(argc < 2){
        printf("wunzip: file1 [file2 ...]\n");
        return 1;
    }


        filePtr = fopen(argv[1],"r");

        if(filePtr == NULL){
            printf("wzip: cannon open file\n");
            return 1;
        }

        fread(&numberOfChars, sizeof(int), 1, filePtr);
        fread(&character, sizeof(char), 1, filePtr);

        printf("%d - %c", numberOfChars, character);
        fclose(filePtr);
    


    return 0;
}