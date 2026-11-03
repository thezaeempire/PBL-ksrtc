#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

int main()
{
// Create shared memory for semaphore
sem_t *sem = mmap(NULL, sizeof(sem_t),
PROT_READ | PROT_WRITE,
MAP_SHARED | MAP_ANONYMOUS,
-1, 0);

```
if (sem == MAP_FAILED)
{
    perror("mmap");
    return 1;
}

// Initialize semaphore
// 1 = Shared between processes
// 0 = Initial semaphore value
if (sem_init(sem, 1, 0) == -1)
{
    perror("sem_init");
    munmap(sem, sizeof(sem_t));
    return 1;
}

printf("Parent: Semaphore initialized.\n");

pid_t pid = fork();

if (pid < 0)
{
    perror("fork");
    sem_destroy(sem);
    munmap(sem, sizeof(sem_t));
    return 1;
}

if (pid == 0)
{
    // Child Process
    printf("Child: Waiting to perform task...\n");

    sleep(2);

    printf("Child: Task completed successfully!\n");

    // Release semaphore
    if (sem_post(sem) == -1)
    {
        perror("sem_post");
        _exit(1);
    }

    _exit(0);
}
else
{
    // Parent Process
    printf("Parent: Waiting for child to complete...\n");

    // Wait for child notification
    if (sem_wait(sem) == -1)
    {
        perror("sem_wait");
        waitpid(pid, NULL, 0);
        sem_destroy(sem);
        munmap(sem, sizeof(sem_t));
        return 1;
    }

    printf("Parent: Child completed its task!\n");

    waitpid(pid, NULL, 0);
}

// Destroy semaphore
if (sem_destroy(sem) == -1)
    perror("sem_destroy");
else
    printf("Semaphore destroyed successfully.\n");

// Release shared memory
if (munmap(sem, sizeof(sem_t)) == -1)
    perror("munmap");

return 0;
```

}
