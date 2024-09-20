#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    // Replace with your own key
    key_t key = ftok(".", 'a');
    if (key == -1) {
        printf("Error generating key\n");
        return 1;
    }

    int msgid = msgget(key, 0666);
    if (msgid == -1) {
        printf("Error getting message queue\n");
        return 1;
    }

    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        printf("Error removing message queue\n");
        return 1;
    }

    printf("Message queue removed successfully\n");
    return 0;
}

/*
--------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./29_out
Message queue removed successfully
--------------------------------------------------------------------
*/



