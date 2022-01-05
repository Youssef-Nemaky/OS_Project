#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h> 
#include <sys/sysinfo.h>

int main(int argc, char * argv[]){
    int fd;
    struct stat sb;
    char * filePtr;
    if(argc < 2){
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    }

    printf("This system has %d processors configured and "
                   "%d processors available.\n",
                   get_nprocs_conf(), get_nprocs());

    /* I just wanna make a simple read using mmap to read a file */
    fd = open(argv[1], O_RDWR);
    
    if(fstat(fd, &sb) == -1){
        printf("Couldn't get file size.\n");
        return 1;
    }

    printf("File size is %ld\n", sb.st_size);

    filePtr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    for(int i = 0; i < sb.st_size; i++){
        printf("%c", filePtr[i]);
    }


    munmap(filePtr, sb.st_size);
    close(fd);

    return 0;
}