#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>

#include "common.h"

int main(void)
{
    mqd_t mq;
    CoreMessage msg;

    /* Open UI -> Core message queue */
    mq = mq_open(UI_TO_CORE, O_WRONLY);

    if (mq == (mqd_t)-1)
    {
        perror("[UI] Failed to open message queue");
        return 1;
    }

    printf("\n====================================\n");
    printf("       MULTI-PROCESS SIMULATOR\n");
    printf("====================================\n");

    /* Get input from user */
    printf("Enter number: ");

    if (scanf("%d", &msg.number) != 1)
    {
        printf("[UI] Invalid number.\n");
        mq_close(mq);
        return 1;
    }

    printf("Enter operation (double/square): ");

    if (scanf("%19s", msg.operation) != 1)
    {
        printf("[UI] Invalid operation.\n");
        mq_close(mq);
        return 1;
    }

    /* Send request to Core */
    if (mq_send(mq, (char *)&msg, sizeof(msg), 0) == -1)
    {
        perror("[UI] Failed to send message");
        mq_close(mq);
        return 1;
    }

    printf("\n[UI] Request sent to Core successfully.\n");
    printf("[UI] Number: %d\n", msg.number);
    printf("[UI] Operation: %s\n", msg.operation);

    mq_close(mq);

    return 0;
}