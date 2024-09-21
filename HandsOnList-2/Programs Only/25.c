/*
============================================================================
Name : 25.c
Author : Aditya Shinde
Description : Write a program to print a message queue's (use msqid_ds and ipc_perm structures)
a. access permission
b. uid, gid
c. time of last message sent and received
d. time of last change in the message queue
d. size of the queue
f. number of messages in the queue
g. maximum number of bytes allowed
h. pid of the msgsnd and msgrcv
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

int main() {
    key_t key;
    int msgid;
    struct msqid_ds mq;

    key = ftok(".", 'a');

    msgid = msgget(key, 0666 | IPC_CREAT);

    msgctl(msgid, IPC_STAT, &mq);

    printf("a.access permission: %o\n", mq.msg_perm.mode);

    printf("b.1.UID: %d\n", mq.msg_perm.uid);
    printf("b.2.GID: %d\n", mq.msg_perm.gid);

    printf("c.1.Time of last message sent: %s", ctime(&mq.msg_stime));
    printf("c.2.Time of last message received: %s", ctime(&mq.msg_rtime));

    printf("d.Time of last change: %s", ctime(&mq.msg_ctime));

    printf("e.Current queue size (in bytes): %lu\n", mq.__msg_cbytes);

    printf("f.Number of messages in queue: %lu\n", mq.msg_qnum);

    printf("g.Maximum number of bytes allowed: %lu\n", mq.msg_qbytes);

    printf("h.1.PID of the msgsnd: %d\n", mq.msg_lspid);
    printf("h.2.PID of the msgrcv: %d\n", mq.msg_lrpid);

    return 0;
}

/*
--------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./25_out
a.access permission: 666
b.1.UID: 1000
b.2.GID: 1000
c.1.Time of last message sent: Thu Jan  1 05:30:00 1970
c.2.Time of last message received: Thu Jan  1 05:30:00 1970
d.Time of last change: Fri Sep 20 17:07:33 2024
e.Current queue size (in bytes): 0
f.Number of messages in queue: 0
g.Maximum number of bytes allowed: 16384
h.1.PID of the msgsnd: 0
h.2.PID of the msgrcv: 0
--------------------------------------------------------------------
*/
