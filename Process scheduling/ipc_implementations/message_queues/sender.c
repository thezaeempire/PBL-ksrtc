
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    msg.msg_type = 1;

    printf("Enter a message to send: ");

    if (fgets(msg.msg_text, sizeof(msg.msg_text), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }

    msg.msg_text[strcspn(msg.msg_text, "\n")] = '\0';

    if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
        perror("msgsnd");
        return 1;
    }

    printf("Sender: Message sent successfully.\n");

    return 0;
}
