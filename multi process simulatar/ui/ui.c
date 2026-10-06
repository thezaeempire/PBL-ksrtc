#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>

#include "../common.h"

int main(void)
{
    mqd_t request_mq;
    mqd_t response_mq;

    char command[MAX_MSG];
    char response[MAX_MSG];

    /* Open queue to send commands to Core */
    request_mq = mq_open(UI_TO_CORE, O_WRONLY);

    if (request_mq == (mqd_t)-1)
    {
        perror("[UI] Failed to open UI-to-Core queue");
        printf("[UI] Make sure Core is running first.\n");
        return 1;
    }

    /* Open queue to receive response from Core */
    response_mq = mq_open(CORE_TO_UI, O_RDONLY);

    if (response_mq == (mqd_t)-1)
    {
        perror("[UI] Failed to open Core-to-UI queue");
        mq_close(request_mq);
        return 1;
    }

    printf("\n====================================\n");
    printf("     MULTI-PROCESS SIMULATOR\n");
    printf("====================================\n");

    printf("\nAvailable commands:\n");
    printf("  ADD 10 20\n");
    printf("  SUB 20 5\n");
    printf("  PUSH 10\n");
    printf("  POP\n");
    printf("  ENQUEUE 10\n");
    printf("  DEQUEUE\n");
    printf("  EXIT\n");

    while (1)
    {
        printf("\nEnter command: ");

        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        if (strlen(command) == 0)
        {
            continue;
        }

        /* Send command to Core */
        if (mq_send(request_mq, command, strlen(command) + 1, 0) == -1)
        {
            perror("[UI] Failed to send command");
            break;
        }

        /* EXIT does not need a response */
        if (strcmp(command, "EXIT") == 0)
        {
            printf("[UI] Exit command sent.\n");
            break;
        }

        /* Wait for Core's response */
        ssize_t bytes = mq_receive(
            response_mq,
            response,
            sizeof(response),
            NULL
        );

        if (bytes == -1)
        {
            perror("[UI] Failed to receive response");
            break;
        }

        response[bytes] = '\0';

        printf("[CORE] %s\n", response);
    }

    mq_close(request_mq);
    mq_close(response_mq);

    return 0;
}
