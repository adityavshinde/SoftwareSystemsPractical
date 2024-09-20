#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

int main() {
    key_t key;
    int msgid;
    struct msqid_ds buf;

    key = ftok(".", 'a');
    msgid = msgget(key, 0666 | IPC_CREAT);
    
    msgctl(msgid, IPC_STAT, &buf);

    printf("Current permissions: %o\n", buf.msg_perm.mode);

    buf.msg_perm.mode = 0644;
    msgctl(msgid, IPC_SET, &buf);

    msgctl(msgid, IPC_STAT, &buf);
    printf("Updated permissions: %o\n", buf.msg_perm.mode);

    return 0;
}

/*
--------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./28_out
Current permissions: 666
Updated permissions: 644
--------------------------------------------------------------------
*/
