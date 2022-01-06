#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h> 
#include <sys/sysinfo.h>
#include <pthread.h>

int numberOfProcessors = 8;
//int numberOfFileThreads = 0;

/* We need a lock to access the global variable to make sure everything is working fine */

pthread_mutex_t buffer_size_lock = PTHREAD_MUTEX_INITIALIZER;
unsigned volatile long long buffer_size = 0;

int * counterArr;

typedef struct{
    int characterCount;
    char chracter;
} zipped_byte_t;

zipped_byte_t ** zippedByteArr;

int num_args;
char * buffer;

typedef struct{
    char * fileName;
    int fd;
    char * filePtr;
    unsigned long long fileSize;
    unsigned int fileIndex;
    unsigned long long bufferStartIndex;
} file_thread_t;

void * fileThread(void * arg){
    file_thread_t * fileStructPtr = (file_thread_t *)arg;
    struct stat sb;
    
    fileStructPtr->fd = open(fileStructPtr->fileName, O_RDWR);

    printf("FD: %d\n", fileStructPtr->fd);

    if(fileStructPtr->fd == -1){
        printf("couldn't open file\n");
        exit(1);
    }    

    if(fstat(fileStructPtr->fd, &sb) == -1){
        printf("Couldn't get file size.\n");
        exit(1);
    } else {
        fileStructPtr->fileSize = sb.st_size;
    }

    printf("File size is %ld\n", sb.st_size);

    /* File size represents number of elements(characters) */
    pthread_mutex_lock(&buffer_size_lock);
    buffer_size = buffer_size + sb.st_size;
    pthread_mutex_unlock(&buffer_size_lock);

    fileStructPtr->filePtr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fileStructPtr->fd, 0);

    for(int i = 0; i < sb.st_size; i++){
        printf("%c", (fileStructPtr->filePtr)[i]);
    }
    return NULL;
}

void * loadBufferThread(void * arg){
    file_thread_t * fileStructPtr = (file_thread_t *)arg;
    unsigned long long start = fileStructPtr->bufferStartIndex;
    unsigned long long end = fileStructPtr->bufferStartIndex + fileStructPtr->fileSize;
    for(unsigned long long i = start; i < end; i++){
        buffer[i] = (fileStructPtr->filePtr)[i - start];
    }
    return NULL;
}

void * compressBufferThread(void * arg){
    int * indexPtr =  (int *)arg;
    int index = *indexPtr;
    printf("Index: %d\n", index);
    int runLength;
    int counter = 0;
    for(int i =  index * (buffer_size / numberOfProcessors),j = i + 1; i < (index + 1) * (buffer_size / numberOfProcessors);){
        runLength = 1;
        while(buffer[i] == buffer[j] && j < (index + 1) * (buffer_size / numberOfProcessors)){
            runLength++;
            j++;
        }
        zippedByteArr[index][counter].characterCount = runLength;
        zippedByteArr[index][counter].chracter = buffer[i];
        counter++;
        i = j;
        j = i + 1;
    }
    counterArr[index] = counter;
    return NULL;
}

/* Brain Storming 
 * 1) use mmap on multiple files by using multiple threads DONE
 * 2) Determine the size of each file DONE
 * 3) Determine the total buffer size DONE
 * 4) Malloc the buffer DONE
 * 5) Read all the files and add them to the buffer DONE
 * 6) Parallel zipping based on the number of threads and the buffer size
 * EX bufferSize = 16 and numOfThreads = 4 -> each thread will work on 4 characters 
 * 7) Merge all the output of all threads
 */
int main(int argc, char * argv[]){
    //printf("%d", get_nprocs());
    //return 0;
    /*
    int fd;
    struct stat sb;
    char * filePtr;
    */
    
    if (argc < 2)
    {
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    }
    int zippedByteLength;

    num_args = argc - 1;

    pthread_t p[num_args];
    file_thread_t pStruct[num_args];
    for(int i = 0; i < num_args; i++){
        pStruct[i].fileName = argv[i + 1];
        pStruct[i].fileIndex = i;
        pthread_create(&p[i], NULL, fileThread, &pStruct[i]);
    }

    for(int i = 0 ; i < num_args; i++){
        pthread_join(p[i], NULL);
    }
    
    printf("in main the buffer size after running the threads is: %llu\n", buffer_size);

    buffer = (char *)malloc(buffer_size * sizeof(char));

    /* Where are we? 
     * We have a buffer of length suitable of containing all the files the user wants to compress
     * What should we do next?
     * PARALLEL read all the files using the memory location mmap provided us with
     * and write all the data to the buffer
     */

    /* Printing starting and ending index for each struct */
    pStruct[0].bufferStartIndex = 0;
    for(int i = 1; i < num_args; i++){
        pStruct[i].bufferStartIndex = pStruct[i - 1].bufferStartIndex + pStruct[i - 1].fileSize;
    }
    
    for(int i = 0; i < num_args;i++){
        //printf("start: %llu end: %llu\n", pStruct[i].bufferStartIndex, pStruct[i].bufferStartIndex + pStruct[i].fileSize - 1);
    }

    //pthread_t p2[num_args];
    for(int i = 0; i < num_args; i++){
        pthread_create(&p[i], NULL, loadBufferThread, &pStruct[i]);
    }

    for(int i = 0; i < num_args; i++){
        pthread_join(p[i], NULL);
    }

    //printf("(((((((((Printing The Buffer:)))))))))\n");

    for(unsigned long long i = 0; i < buffer_size; i++){
        //printf("%c", buffer[i]);
    }



    /* Compressing */
    /* If threads are enough or not for dividing the buffer*/
    if ((buffer_size / numberOfProcessors) * numberOfProcessors != buffer_size)
    {
        /* Make a new array of structures */
        zippedByteLength = numberOfProcessors + 1;
        counterArr = (int *)malloc(zippedByteLength * sizeof(int));
    } else {
        zippedByteLength = numberOfProcessors;
        counterArr = (int *)malloc(zippedByteLength * sizeof(int));
    }
    
    zippedByteArr = malloc(zippedByteLength * sizeof(zipped_byte_t *));
    int indexArr[numberOfProcessors];
    for(int i = 0; i < numberOfProcessors; i++){
        //printf("IN FOR LOOP I = %d\n", i);
        zippedByteArr[i] = (zipped_byte_t *)malloc(2 * (buffer_size / numberOfProcessors) * sizeof(zipped_byte_t));
        indexArr[i] = i;
        pthread_create(&p[i], NULL, compressBufferThread, &indexArr[i]);
    }
    
    //printf("HERE: 1\n");
    for(int i = 0; i < numberOfProcessors; i++){
        pthread_join(p[i], NULL);
    }

    if ((buffer_size / numberOfProcessors) * numberOfProcessors != buffer_size){
        /* Make a new array of structures */
        int counter = 0;
        int runLength = 1;
        zippedByteArr[numberOfProcessors] = (zipped_byte_t *)malloc(2 * (buffer_size / numberOfProcessors) * sizeof(zipped_byte_t));
        for (int i = numberOfProcessors * (buffer_size / numberOfProcessors), j = i + 1; i < buffer_size;)
        {
            runLength = 1;
            while (buffer[i] == buffer[j] && j < buffer_size)
            {
                runLength++;
                j++;
            }
            zippedByteArr[numberOfProcessors][counter].characterCount = runLength;
            zippedByteArr[numberOfProcessors][counter].chracter = buffer[i];
            counter++;
            i = j;
            j = i + 1;
        }
        counterArr[numberOfProcessors] = counter;
    }


   

    //printf("HERE: 2\n");
    
    for(int i = 0; i < zippedByteLength; i++){
        for(int j = 0; j < counterArr[i]; j++){
            printf("%d%c", zippedByteArr[i][j].characterCount, zippedByteArr[i][j].chracter);
        }
        printf("\n");
    }
    
    //printf("HERE: 3\n");
    return 0;
}