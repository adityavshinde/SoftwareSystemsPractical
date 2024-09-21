/*
============================================================================
Name : 24.c
Author : Aditya Shinde
Description : Write a program to create a message queue and print the key and message queue id.
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key;
    int msgid;
    
    key = ftok(".", 'a');

    msgid = msgget(key, 0666 | IPC_CREAT);

    printf("Message Queue Key: %d\n", key);
    printf("Message Queue ID: %d\n", msgid);

    return 0;
}

/*
--------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./24_out
Message Queue Key: 1627930790
Message Queue ID: 0
--------------------------------------------------------------------
*/
