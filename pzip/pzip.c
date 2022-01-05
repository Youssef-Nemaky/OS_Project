#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h> 
#include <sys/sysinfo.h>
#include <pthread.h>
//int numberOfProcessors = get_nprocs();
//int numberOfFileThreads = 0;

typedef struct{
    char * fileName;
    int fd;
    char * filePtr;
    unsigned long long fileSize;
} file_thread_t;

void * fileThread(void * arg){
    file_thread_t * fileStructPtr = (file_thread_t *)arg;
    
    struct stat sb;
    printf("FD: %d\n", fileStructPtr->fd);
    
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

    printf("File size is %ld\n", sb.st_size);

    fileStructPtr->filePtr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fileStructPtr->fd, 0);

    for(int i = 0; i < sb.st_size; i++){
        printf("%c", (fileStructPtr->filePtr)[i]);
    }
    return NULL;
}


/* Brain Storming 
 * 1) use mmap on multiple files by using multiple threads 
 * 2) Determine the size of each file
 * 3) Determine the total buffer size
 * 4) Malloc the buffer
 * 5) Read all the files and add them to the buffer
 * 6) Parallel zipping based on the number of threads and the buffer size
 * EX bufferSize = 16 and numOfThreads = 4 -> each thread will work on 4 characters 
 * 7) Merge all the output of all threads
 */
int main(int argc, char * argv[]){
    /*
    int fd;
    struct stat sb;
    char * filePtr;
    */
    if(argc < 2){
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    }

    pthread_t p;
    file_thread_t pStruct;
    pStruct.fileName = argv[1]; //Assuming only 1 file is passed
    pthread_create(&p, NULL, fileThread, &pStruct);
    pthread_join(p, NULL);
    return 0;
}