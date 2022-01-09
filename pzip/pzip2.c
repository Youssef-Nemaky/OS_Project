#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h> 
#include <sys/sysinfo.h>
#include <pthread.h>
int processorsNumbers;
int threadsNumber;

typedef struct{
    int characterCount;
    char chracter;
} zipped_byte_t;

zipped_byte_t lastZippedByte = {-1, 'a'};
pthread_t * p;
zipped_byte_t ** zippedByteArr;
int * zippedByteLength;
char * filePtr;
long fileSize;
int * indexArr;
void * compressBufferThread(void * arg){
    int * indexPtr = (int * )arg;
    int index = *indexPtr;
    int runLength = 0;
    int counter = 0;
    for(int i =  index * (fileSize / threadsNumber),j = i + 1; i < (index + 1) * (fileSize / threadsNumber);){
        runLength = 1;
        while(filePtr[i] == filePtr[j] && j < (index + 1) * (fileSize / threadsNumber)){
            runLength++;
            j++;
        }
        zippedByteArr[index][counter].characterCount = runLength;
        zippedByteArr[index][counter].chracter = filePtr[i];
        counter++;
        i = j;
        j = i + 1;
    }
    zippedByteLength[index] = counter;
    return NULL;
}

int main(int argc, char * argv[]){
    if(argc < 2){
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }
    processorsNumbers = get_nprocs();
    int fd;
    struct stat sb;
    

    /* loop through the files */
    for(int fileCounter = 1;fileCounter < argc; fileCounter++){
        fd = open(argv[fileCounter], O_RDONLY);
        if (fd == -1)
        {
            printf("couldn't open file\n");
            exit(1);
        }

        if (fstat(fd, &sb) == -1){
            printf("Couldn't get file size.\n");
            exit(1);
        }
        else{
            fileSize = sb.st_size;
        }

        filePtr = mmap(NULL, sb.st_size, PROT_READ , MAP_SHARED, fd , 0);
        if(fileSize < processorsNumbers){
            threadsNumber = 1;
        } else if(fileSize % processorsNumbers != 0){
            threadsNumber = processorsNumbers + 1;
        } else {
            threadsNumber = processorsNumbers;
        }
        zippedByteArr = (zipped_byte_t **)malloc(threadsNumber * sizeof(zipped_byte_t *));
        zippedByteLength = (int *)malloc(threadsNumber * sizeof(int));
        p = (pthread_t *)malloc(threadsNumber * sizeof(pthread_t));
        indexArr = (int *)malloc(threadsNumber * sizeof(int));

        for(int i = 0; i < threadsNumber; i++){
            zippedByteArr[i] = (zipped_byte_t *)malloc(2 * (fileSize / threadsNumber) * sizeof(zipped_byte_t));
            indexArr[i] = i;
            pthread_create(&p[i], NULL, compressBufferThread, &indexArr[i]);
        }
        
        for(int i = 0; i < threadsNumber; i++){
            pthread_join(p[i], NULL);
        }
        /*
        for(int i = 0; i < threadsNumber; i++){
            for(int j = 0; j < zippedByteLength[i]; j++){
                printf("%d%c",zippedByteArr[i][j].characterCount, zippedByteArr[i][j].chracter);
            }
            printf("\n");
        }
        */
        for(int i = 0; i < threadsNumber; i++){
            if (lastZippedByte.characterCount != -1) {
                if (lastZippedByte.chracter == zippedByteArr[i][0].chracter) {
                    zippedByteArr[i][0].characterCount += lastZippedByte.characterCount;
                }
                else {
                    //fwrite(&(lastZippedByte.characterCount), 1, sizeof(int), stdout);
                    //fwrite(&(lastZippedByte.chracter), 1, sizeof(char), stdout);
                    fwrite(&(lastZippedByte), 1, sizeof(zipped_byte_t), stdout);
                }
            }
            
            for(int j = 0; j < zippedByteLength[i] - 1; j++){
                //fwrite(&(zippedByteArr[i][j].characterCount), 1, sizeof(int), stdout);
                //fwrite(&(zippedByteArr[i][j].chracter), 1, sizeof(char), stdout);
                fwrite(&(zippedByteArr[i][j]), 1, sizeof(zipped_byte_t), stdout);
            }
            lastZippedByte.characterCount = zippedByteArr[i][zippedByteLength[i] - 1].characterCount;
            lastZippedByte.chracter = zippedByteArr[i][zippedByteLength[i] - 1].chracter;

            if((fileCounter == argc - 1) && (i == threadsNumber - 1)){
                /* last file */
                //fwrite(&(lastZippedByte.characterCount), 1, sizeof(int), stdout);
                //fwrite(&(lastZippedByte.chracter), 1, sizeof(char), stdout);
                fwrite(&(lastZippedByte), 1, sizeof(zipped_byte_t), stdout);
            }
        }

        munmap(filePtr, fileSize);
        close(fd);
        for(int i = 0; i < threadsNumber; i++){
            free(zippedByteArr[i]);
        }
        free(zippedByteArr);
        free(zippedByteLength);
        free(indexArr);
        free(p);
    }

}