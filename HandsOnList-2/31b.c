/*
============================================================================
Name : 31b.c
Author : Aditya Shinde
Description : Write a program to create a semaphore and initialize value to the semaphore.
b. create a counting semaphore
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

    if (init_sem(semid, 5) == -1) {
        printf("Failed to initialize semaphore\n");
        return 1;
    }

    printf("Counting semaphore created and initialized to 5\n");
    return 0;
}

/*
---------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./31b_out
Counting semaphore created and initialized to 5
---------------------------------------------------------------------
*/
