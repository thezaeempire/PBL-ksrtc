
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_NAME "/my_shared_memory"
#define SIZE 1024

int main()
{
    int fd;
    char *shared_memory;

    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(fd, SIZE) == -1) {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    shared_memory = mmap(NULL, SIZE,
                         PROT_READ | PROT_WRITE,
                         MAP_SHARED, fd, 0);

    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    strcpy(shared_memory,
           "Hello from Member 1 - Shared Memory!");

    printf("Writer: Message written successfully.\n");
    printf("Writer: %s\n", shared_memory);

    munmap(shared_memory, SIZE);
    close(fd);

    return 0;
}
