/*
============================================================================
Name : 30.c
Author : Aditya Shinde
Description : Write a program to create a shared memory.
a. write some data to the shared memory
b. attach with O_RDONLY and check whether you are able to overwrite.
c. detach the shared memory
d. remove the shared memory
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SHM_SIZE 1024  

int main() {
    int shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    char *data = (char *)shmat(shmid, NULL, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    strcpy(data, "Hello, Aditya Here!");
    printf("Data written to shared memory: %s\n", data);

    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }

    char *data_read = (char *)shmat(shmid, NULL, SHM_RDONLY);
    if (data_read == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    printf("Data read from shared memory: %s\n", data_read);

    if (shmdt(data_read) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    printf("Shared memory removed successfully.\n");
    return 0;
}

/*
-------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./30_out
Data written to shared memory: Hello, Aditya Here!
Data read from shared memory: Hello, Aditya Here!
Shared memory removed successfully.
-------------------------------------------------------------------
*/
