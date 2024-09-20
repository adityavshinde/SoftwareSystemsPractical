#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <errno.h>

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key;
    int msgid;
    struct msg_buffer msg;

    key = ftok(".", 'a');
    msgid = msgget(key, 0666 | IPC_CREAT);
    
    int flag = 0;
    printf("Waiting for message ...\n");
    msgrcv(msgid, &msg, sizeof(msg.msg_text), 0, flag);
    printf("Received: %s\n", msg.msg_text);

    printf("Attempting to receive message ...\n");
    if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 0, IPC_NOWAIT) == -1) {
        if (errno == ENOMSG)
            printf("No message available.\n");
        else
            perror("msgrcv failed");
    } 
    else {
        printf("Received: %s\n", msg.msg_text);
    }

    return 0;
}

/*
---------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./27_out
Waiting for message ...
Received: Aditya here!

Attempting to receive message ...
No message available.
---------------------------------------------------------------------
*/

