/*
============================================================================
Name : 32d.c
Author : Aditya Shinde
Description : Write a program to implement semaphore to protect any critical section.
d. remove the created semaphore
Date: 21st Sep, 2024.
============================================================================
*/
#include <sys/types.h> 
#include <sys/ipc.h>   
#include <sys/sem.h>   
#include <unistd.h>    
#include <stdio.h>     

void main()
{
    key_t semKey;
    int semId;
    int semctlStat;

    semKey = ftok(".", 1);
    if (semKey == -1)
    {
        perror("Error while computing key!");
        _exit(1);
    }

    semId = semget(semKey, 1, IPC_CREAT | 0777);

    if (semId == -1)
    {
        perror("Error while creating semaphore!");
        _exit(1);
    }

    printf("Press enter to delete the semaphore!\n");
    getchar();

    semctlStat = semctl(semId, 0, IPC_RMID);

    if (semctlStat == -1)
    {
        perror("Error while deleting semaphore!");
        _exit(0);
    }
}


/*
---------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./32d_out
Press enter to delete the semaphore!

---------------------------------------------------------------------
*/
