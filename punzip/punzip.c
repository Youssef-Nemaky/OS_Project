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
int num_args;

unsigned long long threadBufferSize[8];

typedef struct{
    int characterCount;
    char chracter;
} zipped_byte_t;

pthread_mutex_t buffer_size_lock = PTHREAD_MUTEX_INITIALIZER;
zipped_byte_t * buffer;

char ** threadsOutputArr;

unsigned volatile long long buffer_size = 0;

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

    pthread_mutex_lock(&buffer_size_lock);
    buffer_size = buffer_size + ((sb.st_size)/5);
    pthread_mutex_unlock(&buffer_size_lock);

    fileStructPtr->filePtr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fileStructPtr->fd, 0);
    return NULL;
}

void * loadBufferThread(void * arg){
    file_thread_t * fileStructPtr = (file_thread_t *)arg;
    unsigned long long start = fileStructPtr->bufferStartIndex;
    unsigned long long end = fileStructPtr->bufferStartIndex + ((fileStructPtr->fileSize) / 5) ;

    for(unsigned long long i = start, j = 0; i < end; i++, j += 5){
        buffer[i].characterCount = (*((int *)((fileStructPtr->filePtr) + (j))));
        buffer[i].chracter = *(fileStructPtr->filePtr + (j + 4));
    }
    return NULL;
}


void * decompressBufferThread(void * arg){
    int * indexPtr =  (int *)arg;
    int index = *indexPtr;
    int runLength = 0;
    for(int i =  index * (buffer_size / numberOfProcessors); i < (index + 1) * (buffer_size / numberOfProcessors);i++){
        threadBufferSize[index] += buffer[i].characterCount;
    }
    
    threadsOutputArr[index] = (char *)malloc(threadBufferSize[index] * sizeof(char));

    for(int i =  index * (buffer_size / numberOfProcessors), j = 0; i < (index + 1) * (buffer_size / numberOfProcessors);i++){
        runLength = buffer[i].characterCount;
        while(runLength > 0){
            threadsOutputArr[index][j] = buffer[i].chracter;
            j++;
            runLength--;
            
        }
    }
    return NULL;
}

int main(int argc, char * argv[]){
    
    if(argc < 2){
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }
    int threadsOutputLength;
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
    
    buffer = (zipped_byte_t *)malloc(buffer_size * sizeof(zipped_byte_t));

    threadsOutputArr = (char **)malloc(numberOfProcessors * sizeof(char *));
    pStruct[0].bufferStartIndex = 0;
    for (int i = 1; i < num_args; i++)
    {
        pStruct[i].bufferStartIndex = pStruct[i - 1].bufferStartIndex + ((pStruct[i - 1].fileSize)/5);
    }

    for(int i = 0; i < num_args; i++){
        pthread_create(&p[i], NULL, loadBufferThread, &pStruct[i]);
    }

    for(int i = 0; i < num_args; i++){
        pthread_join(p[i], NULL);
    }

    threadsOutputLength = numberOfProcessors;

    int indexArr[numberOfProcessors];
    for (int i = 0; i < numberOfProcessors; i++){
        indexArr[i] = i;
        pthread_create(&p[i], NULL, decompressBufferThread, &indexArr[i]);
    }

    for (int i = 0; i < numberOfProcessors; i++)
    {
        pthread_join(p[i], NULL);
    }

    

    for (int i = 0; i < threadsOutputLength; i++)
    {
        for (int j = 0; j < threadBufferSize[i]; j++)
        {
            printf("%c", threadsOutputArr[i][j]);
        }
    }

    if ((buffer_size / numberOfProcessors) * numberOfProcessors != buffer_size){
        int runLength = 0;
        for (int i = numberOfProcessors * (buffer_size / numberOfProcessors), j = 0; i < buffer_size;i++){
            runLength = buffer[i].characterCount;
            while (runLength > 0)
            {
                printf("%c",buffer[i].chracter);
                j++;
                runLength--;
            }
        }
    }
}