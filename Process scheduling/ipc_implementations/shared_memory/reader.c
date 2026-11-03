
#include <stdio.h>
#include <stdlib.h>
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

    // Open existing shared memory
    fd = shm_open(SHM_NAME, O_RDONLY, 0666);

    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    // Map shared memory for reading
    shared_memory = mmap(NULL, SIZE,
                         PROT_READ,
                         MAP_SHARED, fd, 0);

    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    // Display received message
    printf("Reader: Reading from shared memory...\n");
    printf("Reader: Received message: %s\n", shared_memory);

    // Unmap shared memory
    if (munmap(shared_memory, SIZE) == -1) {
        perror("munmap");
    }

    close(fd);

    // Remove shared memory object
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        return 1;
    }

    printf("Reader: Shared memory removed successfully.\n");

    return 0;
}
