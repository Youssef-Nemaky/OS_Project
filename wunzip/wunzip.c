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

    for(int i = 0; i < argc - 1; i++){
        filePtr = fopen(argv[i + 1],"r");

        if(filePtr == NULL){
            printf("wzip: cannon open file\n");
            return 1;
        }
        
        while((fread(&numberOfChars, sizeof(int), 1, filePtr) != 0) && (fread(&character, sizeof(char), 1, filePtr) != 0))
        {
            while (numberOfChars > 0){
                printf("%c", character);
                numberOfChars--;
            }
        }
        fclose(filePtr);
    }
    return 0;
}