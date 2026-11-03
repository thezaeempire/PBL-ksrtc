
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY 1234

struct message {
    long msg_type;
    char msg_text[256];
};

int main()
{
    int msgid;
    struct message msg;

    msgid = msgget(KEY, IPC_CREAT | 0666);

    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    printf("Receiver: Waiting for message...\n");

    if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
        perror("msgrcv");
        return 1;
    }

    printf("Receiver: Message received successfully.\n");
    printf("Received message: %s\n", msg.msg_text);

    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        return 1;
    }

    printf("Receiver: Message queue deleted.\n");

    return 0;
}
