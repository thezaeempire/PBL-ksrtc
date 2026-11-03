
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int fd[2];
    pid_t pid;
    char buffer[100];

    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid > 0) {
        close(fd[0]);

        char message[] = "Hello Child! Message from Parent.";

        printf("Parent: Sending message...\n");

        if (write(fd[1], message, strlen(message) + 1) == -1) {
            perror("write");
        }

        close(fd[1]);
        waitpid(pid, NULL, 0);

        printf("Parent: Communication completed.\n");
    }
    else {
        close(fd[1]);

        ssize_t bytes = read(fd[0], buffer, sizeof(buffer));

        if (bytes == -1) {
            perror("read");
            close(fd[0]);
            _exit(1);
        }

        if (bytes > 0) {
            printf("Child: Message received: %s\n", buffer);
        }

        close(fd[0]);
        _exit(0);
    }

    return 0;
}
