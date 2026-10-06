#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

/*
 * These names must match the names used by your team's UI
 * and Logger.
 *
 * Check common.h before final integration.
 */
#define REQUEST_QUEUE   "/pbl_core_request"
#define RESPONSE_QUEUE  "/pbl_core_response"
#define LOG_QUEUE       "/pbl_core_log"

#define MAX_MESSAGE     256
#define STACK_SIZE      100
#define QUEUE_SIZE      100


/* =========================
   STACK
   ========================= */

int stack[STACK_SIZE];
int stack_top = -1;


int push(int value)
{
    if (stack_top >= STACK_SIZE - 1)
    {
        return 0;
    }

    stack_top++;
    stack[stack_top] = value;

    return 1;
}


int pop(int *value)
{
    if (stack_top < 0)
    {
        return 0;
    }

    *value = stack[stack_top];
    stack_top--;

    return 1;
}


/* =========================
   QUEUE
   ========================= */

int queue[QUEUE_SIZE];
int queue_front = 0;
int queue_rear = -1;


int enqueue(int value)
{
    if (queue_rear >= QUEUE_SIZE - 1)
    {
        return 0;
    }

    queue_rear++;
    queue[queue_rear] = value;

    return 1;
}


int dequeue(int *value)
{
    if (queue_front > queue_rear)
    {
        return 0;
    }

    *value = queue[queue_front];
    queue_front++;

    return 1;
}


/* =========================
   CPU OPERATIONS
   ========================= */

int add_numbers(int a, int b)
{
    return a + b;
}


int subtract_numbers(int a, int b)
{
    return a - b;
}


/* =========================
   SEND RESPONSE TO UI
   ========================= */

void send_response(mqd_t response_queue, const char *message)
{
    if (mq_send(response_queue,
                message,
                strlen(message) + 1,
                0) == -1)
    {
        perror("CORE: Failed to send response");
    }
}


/* =========================
   SEND LOG TO LOGGER
   ========================= */

void send_log(mqd_t log_queue, const char *message)
{
    if (mq_send(log_queue,
                message,
                strlen(message) + 1,
                0) == -1)
    {
        perror("CORE: Failed to send log");
    }
}


/* =========================
   MAIN CORE PROCESS
   ========================= */

int main(void)
{
    struct mq_attr attributes;

    attributes.mq_flags = 0;
    attributes.mq_maxmsg = 10;
    attributes.mq_msgsize = MAX_MESSAGE;
    attributes.mq_curmsgs = 0;


    /*
     * Create request queue.
     * UI sends commands here.
     */

    mqd_t request_queue = mq_open(
        REQUEST_QUEUE,
        O_CREAT | O_RDONLY,
        0666,
        &attributes
    );

    if (request_queue == (mqd_t)-1)
    {
        perror("CORE: Cannot open request queue");
        return 1;
    }


    /*
     * Create response queue.
     * Core sends results to UI here.
     */

    mqd_t response_queue = mq_open(
        RESPONSE_QUEUE,
        O_CREAT | O_WRONLY,
        0666,
        &attributes
    );

    if (response_queue == (mqd_t)-1)
    {
        perror("CORE: Cannot open response queue");

        mq_close(request_queue);

        return 1;
    }


    /*
     * Open Logger queue.
     */

    mqd_t log_queue = mq_open(
        LOG_QUEUE,
        O_WRONLY
    );

    if (log_queue == (mqd_t)-1)
    {
        perror("CORE: Cannot connect to Logger");

        printf("CORE: Start the Logger process first.\n");

        mq_close(request_queue);
        mq_close(response_queue);

        return 1;
    }


    printf("\n");
    printf("=================================\n");
    printf("          CORE PROCESS\n");
    printf("=================================\n");
    printf("CPU      : READY\n");
    printf("Stack    : READY\n");
    printf("Queue    : READY\n");
    printf("Logger   : CONNECTED\n");
    printf("=================================\n");
    printf("CORE: Waiting for commands...\n");


    char message[MAX_MESSAGE];


    /*
     * CORE continuously waits for
     * commands from the UI.
     */

    while (1)
    {
        ssize_t received = mq_receive(
            request_queue,
            message,
            MAX_MESSAGE,
            NULL
        );


        if (received == -1)
        {
            perror("CORE: Failed to receive message");
            break;
        }


        message[received] = '\0';


        printf("\nCORE received: %s\n", message);


        /* =========================
           EXIT
           ========================= */

        if (strcmp(message, "EXIT") == 0)
        {
            send_response(
                response_queue,
                "CORE: Shutting down"
            );

            send_log(
                log_queue,
                "CORE: Process shutting down"
            );

            break;
        }


        int a;
        int b;
        int value;
        int result;

        char response[MAX_MESSAGE];
        char log_message[MAX_MESSAGE];


        /* =========================
           ADD
           Example: ADD 10 5
           ========================= */

        if (sscanf(message, "ADD %d %d", &a, &b) == 2)
        {
            result = add_numbers(a, b);

            snprintf(
                response,
                sizeof(response),
                "ADD result = %d",
                result
            );

            send_response(
                response_queue,
                response
            );


            snprintf(
                log_message,
                sizeof(log_message),
                "ADD %d + %d = %d",
                a,
                b,
                result
            );

            send_log(
                log_queue,
                log_message
            );

            continue;
        }


        /* =========================
           SUBTRACT
           Example: SUB 10 5
           ========================= */

        if (sscanf(message, "SUB %d %d", &a, &b) == 2)
        {
            result = subtract_numbers(a, b);

            snprintf(
                response,
                sizeof(response),
                "SUB result = %d",
                result
            );

            send_response(
                response_queue,
                response
            );


            snprintf(
                log_message,
                sizeof(log_message),
                "SUB %d - %d = %d",
                a,
                b,
                result
            );

            send_log(
                log_queue,
                log_message
            );

            continue;
        }


        /* =========================
           PUSH
           Example: PUSH 25
           ========================= */

        if (sscanf(message, "PUSH %d", &value) == 1)
        {
            if (push(value))
            {
                send_response(
                    response_queue,
                    "PUSH successful"
                );

                snprintf(
                    log_message,
                    sizeof(log_message),
                    "PUSH %d",
                    value
                );

                send_log(
                    log_queue,
                    log_message
                );
            }
            else
            {
                send_response(
                    response_queue,
                    "ERROR: Stack overflow"
                );

                send_log(
                    log_queue,
                    "ERROR: Stack overflow"
                );
            }

            continue;
        }


        /* =========================
           POP
           ========================= */

        if (strcmp(message, "POP") == 0)
        {
            if (pop(&value))
            {
                snprintf(
                    response,
                    sizeof(response),
                    "POP result = %d",
                    value
                );

                send_response(
                    response_queue,
                    response
                );


                snprintf(
                    log_message,
                    sizeof(log_message),
                    "POP -> %d",
                    value
                );

                send_log(
                    log_queue,
                    log_message
                );
            }
            else
            {
                send_response(
                    response_queue,
                    "ERROR: Stack is empty"
                );

                send_log(
                    log_queue,
                    "ERROR: Stack is empty"
                );
            }

            continue;
        }


        /* =========================
           ENQUEUE
           Example: ENQUEUE 50
           ========================= */

        if (sscanf(message, "ENQUEUE %d", &value) == 1)
        {
            if (enqueue(value))
            {
                send_response(
                    response_queue,
                    "ENQUEUE successful"
                );

                snprintf(
                    log_message,
                    sizeof(log_message),
                    "ENQUEUE %d",
                    value
                );

                send_log(
                    log_queue,
                    log_message
                );
            }
            else
            {
                send_response(
                    response_queue,
                    "ERROR: Queue is full"
                );

                send_log(
                    log_queue,
                    "ERROR: Queue is full"
                );
            }

            continue;
        }


        /* =========================
           DEQUEUE
           ========================= */

        if (strcmp(message, "DEQUEUE") == 0)
        {
            if (dequeue(&value))
            {
                snprintf(
                    response,
                    sizeof(response),
                    "DEQUEUE result = %d",
                    value
                );

                send_response(
                    response_queue,
                    response
                );


                snprintf(
                    log_message,
                    sizeof(log_message),
                    "DEQUEUE -> %d",
                    value
                );

                send_log(
                    log_queue,
                    log_message
                );
            }
            else
            {
                send_response(
                    response_queue,
                    "ERROR: Queue is empty"
                );

                send_log(
                    log_queue,
                    "ERROR: Queue is empty"
                );
            }

            continue;
        }


        /* =========================
           UNKNOWN COMMAND
           ========================= */

        send_response(
            response_queue,
            "ERROR: Unknown command"
        );


        snprintf(
            log_message,
            sizeof(log_message),
            "ERROR: Unknown command -> %s",
            message
        );

        send_log(
            log_queue,
            log_message
        );
    }


    /* =========================
       CLEANUP
       ========================= */

    mq_close(request_queue);
    mq_close(response_queue);
    mq_close(log_queue);


    printf("\nCORE: Shutdown complete.\n");

    return 0;
}