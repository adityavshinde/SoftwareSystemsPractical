/*
============================================================================
Name : 31a.c
Author : Aditya Shinde
Description : Write a program to create a semaphore and initialize value to the semaphore.
a. create a binary semaphore
Date: 21st Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int init_sem(int semid, int value) {
    return semctl(semid, 0, SETVAL, (int)value);
}

int main() {
    key_t key = ftok(".", 'a');
    if (key == -1) {
        printf("Error generating key\n");
        return 1;
    }

    int semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        printf("Failed to create semaphore\n");
        return 1;
    }

    if (init_sem(semid, 1) == -1) {
        printf("Failed to initialize semaphore\n");
        return 1;
    }

    printf("Semaphore created and initialized successfully\n");
    return 0;
}

/*
---------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./31a_out
Semaphore created and initialized successfully
---------------------------------------------------------------------
*/
