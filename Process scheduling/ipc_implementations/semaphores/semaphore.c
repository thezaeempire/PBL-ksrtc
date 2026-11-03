#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <errno.h>

int main()
{
    sem_t *sem;
    pid_t pid;
    int status;

    // Step 1: Allocate shared memory for semaphore
    sem = mmap(NULL, sizeof(sem_t),
               PROT_READ | PROT_WRITE,
               MAP_SHARED | MAP_ANONYMOUS,
               -1, 0);

    if (sem == MAP_FAILED)
    {
        perror("mmap failed");
        return 1;
    }

    printf("Parent: Shared memory allocated.\n");

    // Step 2: Initialize semaphore
    // 1 = Shared between processes
    // 0 = Initial semaphore value

    if (sem_init(sem, 1, 0) == -1)
    {
        perror("sem_init failed");
        munmap(sem, sizeof(sem_t));
        return 1;
    }

    printf("Parent: Semaphore initialized successfully.\n");

    // Step 3: Create child process
    pid = fork();

    if (pid == -1)
    {
        perror("fork failed");
        sem_destroy(sem);
        munmap(sem, sizeof(sem_t));
        return 1;
    }

    if (pid == 0)
    {
        // CHILD PROCESS

        printf("\nChild: Process started.\n");
        printf("Child: Performing task...\n");
        fflush(stdout);

        sleep(2);

        printf("Child: Task completed successfully!\n");

        // Notify parent process
        if (sem_post(sem) == -1)
        {
            perror("sem_post failed");
            _exit(1);
        }

        printf("Child: Signal sent to parent.\n");
        fflush(stdout);

        _exit(0);
    }
    else
    {
        // PARENT PROCESS

        printf("\nParent: Waiting for child to finish...\n");
        fflush(stdout);

        // Wait until child signals completion
        while (sem_wait(sem) == -1)
        {
            if (errno == EINTR)
                continue;

            perror("sem_wait failed");
            waitpid(pid, NULL, 0);
            sem_destroy(sem);
            munmap(sem, sizeof(sem_t));
            return 1;
        }

        printf("Parent: Child completed its task!\n");

        // Wait for child process to terminate
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid failed");
            sem_destroy(sem);
            munmap(sem, sizeof(sem_t));
            return 1;
        }

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
        {
            printf("Parent: Child process encountered an error.\n");
            sem_destroy(sem);
            munmap(sem, sizeof(sem_t));
            return 1;
        }
    }

    // Step 4: Destroy semaphore
    if (sem_destroy(sem) == -1)
    {
        perror("sem_destroy failed");
        munmap(sem, sizeof(sem_t));
        return 1;
    }

    printf("\nParent: Semaphore destroyed successfully.\n");

    // Step 5: Release shared memory
    if (munmap(sem, sizeof(sem_t)) == -1)
    {
        perror("munmap failed");
        return 1;
    }

    printf("Parent: Shared memory released.\n");

    printf("\nIPC Semaphore demonstration completed!\n");

    return 0;
}